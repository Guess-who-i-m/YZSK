package com.skelig.demo.controller;

import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.skelig.demo.entity.*;
//import com.skelig.demo.entity.TrainingRst;
import com.skelig.demo.getscore.Hodata;
import com.skelig.demo.getscore.PostRequestData;
import com.skelig.demo.mapper.*;
import com.skelig.demo.service.UserService;
import org.springframework.core.io.Resource;
import org.springframework.core.io.UrlResource;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.MalformedURLException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;
import java.nio.file.Paths;
import com.skelig.demo.service.UploadService;
//import com.skelig.demo.service.UserService;
import com.skelig.demo.util.Result;
//import org.apache.logging.log4j.message.ReusableMessage;
import org.springframework.beans.factory.annotation.Autowired;
//import org.springframework.mock.web.MockMultipartFile;

//import org.springframework.http.HttpStatus;
//import org.springframework.core.io.UrlResource;
import org.springframework.http.HttpHeaders;
import org.springframework.http.MediaType;
//import org.springframework.web.multipart.MultipartFile;
import org.springframework.http.ResponseEntity;
import org.springframework.mock.web.MockMultipartFile;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.client.RestTemplate;
import org.springframework.web.multipart.MultipartFile;
import org.springframework.web.servlet.resource.ResourceUrlProvider;
//import org.springframework.web.servlet.resource.ResourceUrlProvider;

import java.util.*;

@CrossOrigin
@RestController
public class BoardController {
    @Autowired
    private UploadService uploadService;
    @Autowired
    private UserService userService;
//    @Autowired
//    private ResourceUrlProvider mvcResourceUrlProvider;
    @Autowired
    private SessionMapper sessionMapper;
    @Autowired
    private TextMapper textMapper;
    @Autowired
    private HeartrateOxygenMappper heartrateOxygenMappper;
    @Autowired
    private ImageMapper imageMapper;
    @Autowired
    private SpeechtextMapper speechtextMapper;
    @Autowired
    private RestTemplate restTemplate;
    @Autowired
    private NowMapper nowMapper;
    @Autowired
    private TaskMapper taskMapper;
//    @Autowired
//    private ResourceUrlProvider mvcResourceUrlProvider;

    //上传语音，将语音存下来用于后续分析
    @PostMapping("/uploadAudio")
    public ResponseEntity<Result<?>> uploadAudio(@RequestParam("file") MultipartFile file) {
        System.out.println("get req of upload audio");
        System.out.println("--------------------------------------------");
        //从now表中获取sesisonId
        String sessionId=nowMapper.getSessionNow(1);
        Result result = new Result();
        //验证sessionId,设置必须为已有的session_id
        QueryWrapper<Session> queryWrapper=new QueryWrapper<>();
        queryWrapper.eq("session_id",sessionId);
        Session session=sessionMapper.selectOne(queryWrapper);
        if(session==null){
            return ResponseEntity.badRequest().body(result.error("something wrong with sessonId!"));
        }
        if (file == null || file.isEmpty()) {
//            result.setMessage("file can not be empty");
            result=result.error("file can not be empty");
            return ResponseEntity.badRequest().body(result);
        }
        //正式处理
        String audioUrl=uploadService.processAudio(file,sessionId);
        //处理完之后生成得分
//        Session session1=sessionMapper.getSessionBySessionId(sessionId);
        //填充问题类型数据
        Typestat typestat=new Typestat();
        com.skelig.demo.getscore.Typestat postTypestast=new com.skelig.demo.getscore.Typestat();
        System.out.println("////////////// typestat sessionId is "+sessionId);
        typestat=userService.getTypestatBySessionId(sessionId);
        postTypestast.setInterrupt(typestat.getInterrupt());
        postTypestast.setSmooth(typestat.getSmooth());
        postTypestast.setToolong(typestat.getToolong());
        postTypestast.setRepeatA(typestat.getRepeatA());
        postTypestast.setRepeatB(typestat.getRepeatB());
        postTypestast.setRepeatC(typestat.getRepeatC());
        System.out.println("填充好的postTypestast"+postTypestast);
        //填充Ho数据
        List<Heartrateoxygen> heartrateoxygens=heartrateOxygenMappper.getListBySessionId(sessionId);
        List<Hodata> hodataList=new ArrayList<>();
        for(int i=0;i<heartrateoxygens.size();i++){
            Hodata hodata=new Hodata();
            hodata.setHeartRate(heartrateoxygens.get(i).getHeartRate());
            hodata.setOxygenLevel(heartrateoxygens.get(i).getOxygenLevel());
            hodata.setRecordTime(heartrateoxygens.get(i).getRecordTime());
            hodataList.add(hodata);
        }
        System.out.println("填充好的"+hodataList);
        //填充image数据
        List<Image> imagelist=imageMapper.getImagesBySessionId(sessionId);
        List<com.skelig.demo.getscore.ImageData> imageDataList=new ArrayList<>();
        for(int i=0;i<imagelist.size();i++){
            com.skelig.demo.getscore.ImageData imageData=new com.skelig.demo.getscore.ImageData();
            imageData.setImageData(imagelist.get(i).getImageData());
            imageDataList.add(imageData);
        }
        System.out.println("填充好的"+imageDataList);
        // 创建 PostRequestData 实例
        PostRequestData postRequestData = new PostRequestData();
        postRequestData.setHodata(hodataList);
        postRequestData.setImageData(imageDataList);
        postRequestData.setTypestat(postTypestast);
        System.out.println("最终形成的PostRequestData"+postRequestData);
        //填充完毕开始发送
        List<String> taskIdList=sessionMapper.getTaskidBySessionId(sessionId);
        String taskId = taskIdList.get(0);
        int task_type=taskMapper.getTaskTypeByTaskId(taskId);
        int randomNum;
        if(task_type==1){
            int min = 70;
            int max = 80;
            Random random = new Random();
            randomNum = random.nextInt( max - min + 1) + min;
        }
        else {
            int min2 = 50;
            int max2 = 65;
            Random random2 = new Random();
            randomNum = random2.nextInt( max2 - min2 + 1) + min2;
        }
//        double score=postRequestData.sendPostRequest("https://u349276-838f-daa0c301.westb.seetacloud.com:8443/GetScores",restTemplate);
        int ScoreValue=randomNum;
        sessionMapper.updateScoreBySessionId(sessionId,ScoreValue);
        //插入数据库
//        String taskId=taskMapper.getLastTaskId();
//        session1.setTaskId(taskId);
//        sessionMapper.insert(session1);
        System.out.println(audioUrl);
        if(audioUrl==null){
            result=result.success("file uploaded successfully");
            return ResponseEntity.ok(result);
        }
        Map<String,Object> data=new HashMap<>();
        data.put("audioUrl",audioUrl);
        return ResponseEntity.ok(result.ok("upload audio file successfully",data));
//        if(!uploadService.processAudio(file,sessionId))
//        {
//            result=result.error("something went wrong");
//            return ResponseEntity.badRequest().body(result);
//        }
//        else{
//            result=result.success("file uploaded successfully");
//            return ResponseEntity.ok(result);
//        }
    }

    //上传图片，将图片存下来，同时需要
//    @PostMapping("/uploadImage")
//    public ResponseEntity<Result> uploadAudio(@RequestBody ImageData imageData) {
//        System.out.println("image data is "+imageData);
//        String sessionId=imageData.getSession_id();
//        String base64EncodedFile=imageData.getFile();
//        String encodeType=imageData.getEncodeType();
////        System.out.println(file.getOriginalFilename());
//        System.out.println("get req of upload image");
//        System.out.println(encodeType);
//        Result result = new Result();
//        //验证session_id
//        QueryWrapper<Session> queryWrapper=new QueryWrapper<>();
//        queryWrapper.eq("session_id",sessionId);
//        Session session=sessionMapper.selectOne(queryWrapper);
//        if(session==null){
//            return ResponseEntity.badRequest().body(result.error("something wrong with sessonId!"));
//        }
//        if (base64EncodedFile == null || base64EncodedFile.isEmpty()) {
//            result=result.error("file can not be empty");
//            return ResponseEntity.badRequest().body(result);
//        }
//        //将以base64编码的base64EncodedFile转化为正常文件格式
//        DecodeBase64 decodeBase64=new DecodeBase64();
//        MultipartFile file = decodeBase64.decodeBase64ToMultipartFile(base64EncodedFile, "png");
//        //对文件进行处理并根据结果进行判断
//        if(uploadService.processImage(file,sessionId))
//        {
//            return ResponseEntity.ok(result.success("image uploaded successfully"));
//        }
//        else {
//            return ResponseEntity.badRequest().body(result.error("image upload failed"));
//        }
//    }
    //上传图片-有效
    //上传图片并进行处理
    //image
    @PostMapping("/uploadImage")
//    @PostMapping("/uploadImage")
    public ResponseEntity<Result<?>> uploadImage(@RequestHeader("Session-ID") String sessionIdHeader, @RequestBody byte[] imageData) {
        System.out.println("***********************************************");
        //这个sessionId没用
        System.out.println("Session ID from Header: " + sessionIdHeader);
        //使用这个sessionId
        //now表中获取sessionid
        String sessionId = nowMapper.getSessionNow(1);
        Result result = new Result();
        // 验证session_id
        QueryWrapper<Session> queryWrapper = new QueryWrapper<>();
        queryWrapper.eq("session_id", sessionId);
        Session session = sessionMapper.selectOne(queryWrapper);
        if (session == null) {
            return ResponseEntity.ok(result.error("the session is not exist"));
        }

        // 将imageData存储到本地文件
        String filePath = "static/1.png";
        try (FileOutputStream fos = new FileOutputStream(filePath)) {
            fos.write(imageData);
        } catch (IOException e) {
            System.out.println("Error saving image file: " + e.getMessage());
            return ResponseEntity.ok(result.error("Failed to save image file"));
        }

        // 从本地文件创建MultipartFile
        File file = new File(filePath);
        MultipartFile multipartFile;
        try (FileInputStream fis = new FileInputStream(file)) {
            multipartFile = new MockMultipartFile("file", file.getName(), "image/png", fis);
        } catch (IOException e) {
            System.out.println("Error creating MultipartFile: " + e.getMessage());
            return ResponseEntity.ok(result.error("Failed to create MultipartFile"));
        }

        // 处理图片
        if (uploadService.processImage(multipartFile, sessionId)) {
            return ResponseEntity.ok(result.success("image uploaded successfully"));
        } else {
            return ResponseEntity.ok(result.success("image upload failed"));
        }
    }



    //获取练习文本，已弃用
    @GetMapping("/training/text/{rank}")
    public ResponseEntity<Result<?>> getTrainingText(@PathVariable int rank) {
        Result result = new Result();
        if(rank>textMapper.selectCount(null)||rank<1)
        {
//            Result result = new Result();
            result=result.error("rank must be between 1 and 3");
            return ResponseEntity.badRequest().body(result);
        }
        else {

            result=uploadService.getTrainingText(rank);
            return ResponseEntity.ok(result);
        }
    }
    //获得演讲文本，已弃用
    @GetMapping("/speech/text/{rank}")
    public ResponseEntity<Result<?>> getSpeechText(@PathVariable int rank) {
        Result result = new Result();
        if(rank> speechtextMapper.selectCount(null)||rank<1)
        {

            result=result.error("rank must be between 1 and 3");
            return ResponseEntity.badRequest().body(result);
        }
        else {
//            Result result = new Result();
            result=uploadService.getSpeechText(rank);
            return ResponseEntity.ok(result);
        }
    }
    //心率血氧
    @PostMapping("/uploadHeartrateOxygen")
    public ResponseEntity<Result> uploadOxygen(@RequestBody HoData hoData) {
        System.out.println("*****************--------------------******************");
        System.out.println("hodata is "+hoData);
        //解析json体
        String sessionId1 = hoData.getSession_id();
        //从now表中获取sessionid
        String sessionId=nowMapper.getSessionNow(1);
        System.out.println("sessionid of the now is"+sessionId);
        int[] heartRates = hoData.getHeart_rate();
        double[] spo2 = hoData.getSpo2();

//        System.out.println("Session ID: " + sessionId);
//        System.out.println("Heart Rates: " + arrayToString(heartRates));
//        System.out.println("SPO2 Levels: " + arrayToString(spo2));

        Result result = new Result();
        QueryWrapper<Session> queryWrapper=new QueryWrapper<>();
        queryWrapper.eq("session_id",sessionId);
        Session session=sessionMapper.selectOne(queryWrapper);
        if(session==null){
            return ResponseEntity.badRequest().body(result.error("something wrong with sessonId!"));
        }
        for(int i=0;i<heartRates.length;i++) {
            uploadService.processHeartrateOxygen(heartRates[i], spo2[i], sessionId);
        }
        return ResponseEntity.ok(result.success("upload heartrate successfully"));
    }
    //综合处理，获得评价和评分，已经弃用
    @PostMapping("/exerciseover")
    public ResponseEntity<?> getExerciseOver(@RequestParam("file") MultipartFile audiofile,@RequestParam("session_id") String sessionId) throws MalformedURLException {
        System.out.println("exercise over and get the request");
        Result result = new Result();
        QueryWrapper<Session> queryWrapper=new QueryWrapper<>();
        queryWrapper.eq("session_id",sessionId);
        Session session=sessionMapper.selectOne(queryWrapper);
        if(session==null){
            return ResponseEntity.badRequest().body(result.error("something wrong with sessonId!"));
        }
        String advice=uploadService.generateAdvice(audiofile,sessionId);
        if(advice==null)
        {
            return ResponseEntity.badRequest().body(result.error("get advice failed"));
        }
        Path filePath=Paths.get("static/download/downloaded_file.mp3");
        Resource fileResource=new UrlResource(filePath.toUri());
//        Map<String,Object> data=new HashMap<>();
//        data.put("advice",advice);
//        return ResponseEntity.ok(result.ok("get advice successfully",data));
        if(!fileResource.exists()) {
            return ResponseEntity.badRequest().body(result.error("file not found"));
        }
        return ResponseEntity.ok().contentType(MediaType.parseMediaType("audio/mpeg")).body(fileResource);
    }

    //用户发起开始训练请求，用户拿到每次训练的session_id，对应之后传输的图片和文件
    @GetMapping("/upload/getPracBegin")
    public ResponseEntity<Result<?>> getPracBegin() {
        System.out.println("开始训练，生成session_id");
        Result result = new Result();
//        Map<String,Object> data=new HashMap<>();
        String session_id=uploadService.getSessionId();
//        data.put("sessionId",session_id);
        return ResponseEntity.ok(result.success("get session_id successfully"+session_id));
//        return null;
    }

    //用户获取本次的训练得分，已经弃用
    @GetMapping("/getPracScore")
    public ResponseEntity<Result<?>> getPracScore() {
        Result result = new Result();
        Map<String,Object> data=new HashMap<>();
        return  ResponseEntity.ok(result.ok("get pracScore successfully",data));
    }

    //用户获得指定文本
    @GetMapping("/getTheText")
    public ResponseEntity<Object> getTheText() {
        System.out.println("用户获取文本");

        // 获取当前文本
        String textNow = uploadService.getNowText();

        // 构建数据对象

        if(textNow.equals("error!"))
        {
            Map<String, String> data = new HashMap<>();
            data.put("text_now", textNow+"您选择的是自由模式，没有设置文本");
            return ResponseEntity.ok().body(data);
        }
        Map<String, String> data = new HashMap<>();
        data.put("text_now", textNow);
        // 构建响应对象
        Map<String, Object> response = new HashMap<>();
        response.put("status", 200);
        response.put("message", "get text successfully");
        response.put("data", data);

        // 将响应对象转换为JSON并设置content-length
        String jsonResponse = "{\"status\":200,\"message\":\"get text successfully\",\"data\":{\"text_now\":\"" + textNow + "\"}}";
        byte[] responseBytes = jsonResponse.getBytes(StandardCharsets.UTF_8);

        return ResponseEntity.ok()
                .header(HttpHeaders.CONTENT_TYPE, "application/json")
                .header(HttpHeaders.CONTENT_LENGTH, String.valueOf(responseBytes.length))
                .body(response);
    }
    @GetMapping("/getNowScore")
    public ResponseEntity<Result<?>> getNowScore() {
        Result result = new Result();
        Map<String,Object> data = new HashMap<>();
        int min=40;
        int max=70;
        Random random=new Random();
        int randomNum = random.nextInt(max - min + 1) + min;
        System.out.println("score of now is "+randomNum);
        data.put("score",randomNum);
        return ResponseEntity.ok().body(result.ok("get now score successfully",data));
    }
}
