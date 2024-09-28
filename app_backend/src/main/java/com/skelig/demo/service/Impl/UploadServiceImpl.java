package com.skelig.demo.service.Impl;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import com.skelig.demo.entity.*;
import com.skelig.demo.mapper.*;
import com.skelig.demo.service.UploadService;
import com.skelig.demo.util.AskForAdvice;
import com.skelig.demo.util.ProcessImage;
import com.skelig.demo.util.Result;
import com.skelig.demo.util.UpLoadFile;
import io.netty.handler.timeout.TimeoutException;
import io.netty.resolver.DefaultAddressResolverGroup;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.core.io.buffer.DataBuffer;
import org.springframework.core.io.buffer.DataBufferUtils;
import org.springframework.core.io.buffer.DefaultDataBufferFactory;
import org.springframework.http.HttpHeaders;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.http.client.reactive.ReactorClientHttpConnector;
import org.springframework.stereotype.Service;
import org.springframework.web.client.RestTemplate;
import org.springframework.web.multipart.MultipartFile;
import org.springframework.web.reactive.function.BodyInserters;
import org.springframework.web.reactive.function.client.WebClient;
import reactor.core.publisher.Flux;
import reactor.netty.http.client.HttpClient;
import reactor.netty.http.client.PrematureCloseException;
import reactor.util.retry.Retry;
//import java.io.File;
import java.io.IOException;
import java.lang.reflect.Type;
import java.time.Duration;
import java.util.*;

@Service
public class UploadServiceImpl implements UploadService {
    @Autowired
    private LogMapper logMapper;
    @Value("${upload.dir:static/audioUploads/}")
    private String uploadDirAudio; // 使用Spring的@Value注解来配置文件上传目录espdatabase
    @Autowired
    private PhotoMapper photoMapper;
    @Autowired
    private TypestatMapper typestatMapper;
    @Autowired
    private NowMapper nowMapper;
    @Autowired
    private TextMapper textMapper;
    @Autowired
    private TaskMapper taskMapper;
    @Autowired
    private SessionMapper sessionMapper;
    @Autowired
    private SpeechtextMapper speechtextMapper;
    @Autowired
    private AudioMapper audioMapper;
    @Autowired
    private RestTemplate restTemplate;
    private final WebClient webClient;
//    private final AudioMapper audioMapper;
//    private final TypestatMapper typestatMapper;

    @Autowired
    public UploadServiceImpl(AudioMapper audioMapper, TypestatMapper typestatMapper) {
        this.audioMapper = audioMapper;
        this.typestatMapper = typestatMapper;
        this.webClient = WebClient.builder()
                .clientConnector(new ReactorClientHttpConnector(
                        HttpClient.create()
                                .resolver(DefaultAddressResolverGroup.INSTANCE)
                                .responseTimeout(Duration.ofSeconds(30))  // 设置响应超时
                ))
                .baseUrl("https://u349276-838f-daa0c301.westb.seetacloud.com:8443")
                .build();
    }

    //音频文件上传到云，同时插入数据库中,还需修改
    @Override
    public String processAudio(MultipartFile audiofile, String sessionId) {
        //新建问题类
        Typestat typestat = new Typestat();
        //音频库中新建
        Audio audio = new Audio();
        audio.setSessionId(sessionId);
        try {
            //上传到云上
            String audioUrl = UpLoadFile.upLoadAudio(audiofile);
            System.out.println("****----上传的url:"+audioUrl+"----*****");
            audio.setAudioUrl(audioUrl);
            audioMapper.insert(audio); // 插入音频记录到数据库

            // 准备文件数据使用 DataBuffer
            Flux<DataBuffer> content = DataBufferUtils.readInputStream(
                    audiofile::getInputStream,
                    new DefaultDataBufferFactory(),
                    8192
            );

            // 发送 POST 请求并处理响应
            String response = webClient.post()
                    .uri("/FluentNet")
                    .header(HttpHeaders.CONTENT_TYPE, MediaType.MULTIPART_FORM_DATA_VALUE)
                    .body(BodyInserters.fromMultipartData("audio_file", audiofile.getResource()))
                    .retrieve()
                    .bodyToMono(String.class)
                    .retryWhen(Retry.backoff(3, Duration.ofSeconds(8))  // 重试机制
                            .filter(throwable -> throwable instanceof TimeoutException ||
                                    throwable instanceof PrematureCloseException))
                    .block(); // 阻塞线程直到收到响应

            // 解析响应
            Gson gson = new Gson();
            Type listType = new TypeToken<List<Integer>>(){}.getType();
            List<Integer> values = gson.fromJson(response, listType);

            if (values != null && values.size() >= 6) {
                typestat.setSmooth(values.get(0));
                typestat.setInterrupt(values.get(1));
                typestat.setRepeatA(values.get(2));
                typestat.setRepeatB(values.get(3));
                typestat.setRepeatC(values.get(4));
                typestat.setToolong(values.get(5));
            }
            System.out.println("修改前："+typestat);

            int min = 0;
            int max = 2;
            Random random = new Random();
            int randomNum = random.nextInt(max - min + 1) + min;

            List<String> taskIdList=sessionMapper.getTaskidBySessionId(sessionId);
            String taskId = taskIdList.get(0);
            int task_type=taskMapper.getTaskTypeByTaskId(taskId);
            if(task_type==1)
            {
                int temp=typestat.getSmooth();
                typestat.setSmooth(typestat.getRepeatB());
                typestat.setRepeatB(temp);
                System.out.println("修改后："+typestat);
            }

            typestat.setSessionId(sessionId);
            typestatMapper.insert(typestat); // 插入统计数据到数据库
            System.out.println("Server response: " + response);
            return audioUrl; // 返回音频URL

        } catch (IOException e) {
            throw new RuntimeException("Failed to process and send audio file", e);
        }
    }



    public Result<?> getTrainingText(int rank)  {
        //获取文本
        String textData= String.valueOf(textMapper.selectById(rank).getTextContent());
        //生成session_id
        long timestamp=System.currentTimeMillis();
        String sessionId="sess_"+timestamp;
        //插入数据库
        Session session=new Session();
        session.setSessionId(sessionId);
        sessionMapper.insert(session);
        session.setCreatedAt(new Date());
        //记录表中插入新的session_id，每条记录的session_id有且只有一个
        Log log=new Log();
        log.setSessionId(sessionId);
        log.setUserId(1);
        log.setTextId(rank);
        log.setTextType(1);//1表示为基础训练文本
        logMapper.insert(log);
        //创建返回数据
        Map<String,Object> data=new HashMap<>();
        data.put("sessionId",sessionId);
        data.put("textData",textData);
        return Result.ok("Session created and text retrieved successfully",data);
    }
    //获得演讲文本

    public Result<?> getSpeechText(int rank) {
        //训练文本部分可以后面进行处理
        //获取文本
        String textData= String.valueOf(speechtextMapper.selectById(rank).getText());
//        return new Result<>().ok("获取文本成功",textData);
        //生成session_id
        long timestamp=System.currentTimeMillis();
        String sessionId="sess_"+timestamp;
        //插入数据库
        Session session=new Session();
        session.setSessionId(sessionId);
        sessionMapper.insert(session);
        session.setCreatedAt(new Date());
        //记录表中插入新的session_id，每条记录的session_id有且只有一个
        Log log=new Log();
        log.setSessionId(sessionId);
        log.setUserId(1);
        log.setTextId(rank);
        log.setTextType(2);//2表示演讲文本
        logMapper.insert(log);
        //创建返回数据
        Map<String,Object> data=new HashMap<>();
        data.put("sessionId",sessionId);
        data.put("textData",textData);
        return Result.ok("Session created and text retrieved successfully",data);
    }

    @Value("${upload.dir:static/imageUploads/}")
    private String uploadDirImage; // 使用Spring的@Value注解来配置文件上传目录
    @Override
    //处理图片
    public boolean processImage(MultipartFile imagefile, String sessionId) {

        if (imagefile.isEmpty()) {
            return false;
        }
        try {
            //上传文件，获得阿里云返回的文件url
            String  fileName=UpLoadFile.upLoadImage(imagefile);
            ProcessImage processImage=new ProcessImage();
            //算法处理，获得信息
            String mood=processImage.getImageData(imagefile);
            System.out.println("==========处理图片使用的sessionID========"+sessionId);
            // 创建Image对象并设置相关属性
            Image image = new Image();
            image.setImageData(mood);//这是图片信息为识别出的表情
            image.setSessionId(sessionId);//标识图片为第几次的训练
//            image.setImageId(fileName);  // 这里使用生成的文件名作为imageId
            image.setImageUrl(fileName);  // 设置图片的阿里云URL路径，方便后续进行统一读取
            System.out.println("image_url:"+image.getImageUrl());
            // 插入数据库
            photoMapper.insert(image);
            return true;
        } catch (IOException e) {
            // 在这里应该记录日志
            System.out.println("Error processing image file: " + e.getMessage());
            return false;
        }
    }

    @Autowired
    private HeartrateOxygenMappper heartrateMappper;
    public void processHeartrateOxygen(int heartrate,double oxygen,String sessionId)
    {
        Heartrateoxygen heartrateoxygen=new Heartrateoxygen();
        heartrateoxygen.setOxygenLevel(oxygen);
        heartrateoxygen.setSessionId(sessionId);
        heartrateoxygen.setHeartRate(heartrate);
        heartrateMappper.insert(heartrateoxygen);
//        return true;
    }
//    @Autowired
//    private RestTemplate restTemplate;
    //处理用户发送来的音频文件，需要接收sessionId表明为第几次
    @Override
    public String generateAdvice(MultipartFile audiofile,String sessionId) {
        AskForAdvice askForAdvice=new AskForAdvice();
        String advice= askForAdvice.uploadFileAndParseResponse(audiofile);
        System.out.println("advice:"+advice);
        //正确返回建议
        if(advice!=null)
        {
            //此处可以根据图片的信息添加相关的反馈信息
            //根据sessionId存入数据库
            logMapper.updateAdviceBySessionId(sessionId,advice);
            //发送delete请求，删除文件
            String url = "https://u349276-a810-a83d2396.westc.gpuhub.com:8443/delete";
            ResponseEntity<String> response = restTemplate.getForEntity(url, String.class);
            System.out.println("Status Code: " + response.getStatusCode());
            System.out.println("Response Body: " + response.getBody());
            return advice;
        }
        return null;
    }
    //生成sessionId，now表中更新sessionId，更新session表
    @Override
    public String getSessionId() {
        //生成session_id
        long timestamp=System.currentTimeMillis();
        String sessionId="sess_"+timestamp;
        //更新now表，更新现在使用的sesison_id
        nowMapper.updateSessionId(sessionId);
        System.out.println("更新后的sesionid"+sessionId);
        //插入数据库
        Session session=new Session();
        session.setSessionId(sessionId);
        //获得task_id，
        String task_id=taskMapper.getLastTaskId();
        session.setTaskId(task_id);
        session.setCreatedAt(new Date());
        sessionMapper.insert(session);
        //记录表中同样也需要插入
//        Log log=new Log();
//        log.setSessionId(sessionId);
//        log.setUserId(1);
//        logMapper.insert(log);
        //返回sessionId即可
        return sessionId;
    }
    //根据now表中对应rank，获取text表中对应的数据，
    @Override
    public String getNowText() {
        int rank=nowMapper.getRankNow(1);
        System.out.println("得到现在的文本 rank is "+rank);
        if(rank==0)
        {
            return "error!";
        }else{
            Text text=textMapper.getTextById(rank);
            System.out.println(text);
            System.out.println(text.getTextContent());
            return text.getTextContent();
        }

    }


//    public Result<?> generateAdvice(String session_id)
//    {
//        return
//    }
}
