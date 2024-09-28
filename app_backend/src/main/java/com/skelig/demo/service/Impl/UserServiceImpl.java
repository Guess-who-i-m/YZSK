package com.skelig.demo.service.Impl;

import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.skelig.demo.entity.*;
import com.skelig.demo.mapper.*;
import com.skelig.demo.service.UserService;
import com.skelig.demo.util.Result;
import org.mybatis.spring.annotation.MapperScan;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.io.Resource;
import org.springframework.core.io.UrlResource;
import org.springframework.stereotype.Service;

import java.io.IOException;
import java.nio.file.DirectoryStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;
import java.util.stream.Collectors;

@MapperScan("com.skelig.demo.mapper")
@Service
public class UserServiceImpl implements UserService {
    @Autowired
    private UserMapper userMapper;
    @Autowired
    private SessionMapper sessionMapper;
    @Autowired
    private NowMapper nowMapper;
    @Autowired
    private LogMapper  logMapper;
    @Autowired
    private TextMapper textMapper;
    @Autowired
    private TaskMapper taskMapper;
    @Autowired
    private ImageMapper imageMapper;
    @Autowired
    private TypestatMapper typestatMapper;
//    @Autowired
//    private ResourceUrlProvider mvcResourceUrlProvider;
    @Autowired
    private SpeechtextMapper speechtextMapper;
    @Autowired
    private HeartrateOxygenMappper heartrateOxygenMappper;
    @Autowired
    private PhotoMapper photoMapper;

    @Override
    //查询返回所用用户
    public List<User> searchAllUser()
    {
       return userMapper.selectList(null);
    }

    @Override
    public Result<?> getScoreCharts() {
        //按时间从小到大排序的时间数组
        QueryWrapper<Session> queryWrapper = new QueryWrapper<>();
        queryWrapper.select("created_at").orderByAsc("created_at");
        List<Session> sessions = sessionMapper.selectList(queryWrapper);//返回一个只包含created_at的session数组
        List<Date> dateList=sessions.stream().map(Session::getCreatedAt).collect(Collectors.toList());//得到created_at数组，按照时间从小到大排序
        //按照时间顺序得到score数组
        QueryWrapper<Log> queryWrapper1=new QueryWrapper<>();
        queryWrapper.select("score").orderByAsc("id");
        List<Log> logs= logMapper.selectList(queryWrapper1);
//        List<Integer> scoreList=logs.stream().map(Log::getScore).collect(Collectors.toList());//得到score数组
        //得到对应练习文本数组
//        List<>
        //构造返回数据
        Map<String,List<?>> data=new HashMap<>();
        data.put("datelist",dateList);
        data.put("scorelist",logs);
//        System.out.println(sessions);
        return Result.ok("获取数据成功！",data);
    }
    //得到基础训练表中的所有内容：训练文本，练习种类，以及id
    public List<Text> generateBasicTexts(){
        return textMapper.selectList(null);
    }

    @Override
    public boolean processSpeechText(String text) {
        try{
            Speechtext speechtext=new Speechtext();
            speechtext.setText(text);
            speechtextMapper.insert(speechtext);
            return true;
        }
        catch (Exception e){
            e.printStackTrace();
            return false;
        }

    }

    @Override
    public List<Speechtext> generateSpeechTexts() {
        return speechtextMapper.selectList(null);
    }

    @Override
    public Result<?> generateDetail(int id) {
        Map<String,Object> data=new HashMap<>();
        //1.拿到对应的session_id用于比较
        String theSessionId=sessionMapper.getSessionIdById(id);
        System.out.println("theSessionId:"+theSessionId);
        //2.根据session_id拿到对应的得分和评价,训练文本种类，训练文本序号
        Log log=logMapper.getLogDetailsBySessionId(theSessionId);//这个log用于暂时保存对应信息
        System.out.println(log);
        //装填
        data.put("score",log.getScore());
        data.put("advice",log.getAdvice());
        data.put("BasicorSpeech",log.getTextType());
        //3.根据对应文本获取演讲文本或者训练文本
        //训练文本
        if(log.getTextType()==1)
        {
            Text text=textMapper.getTextById(log.getTextId());
            System.out.println("text:"+text);
            //装填
            data.put("BasicTextType",text);
        }
        else if(log.getTextType()==2)
        {
            String SpeechText=speechtextMapper.getTextById(log.getTextId());
            System.out.println("SpeechText:"+SpeechText);
        }
        //4.根据session_id获得对应的心率，血氧信息
        List<Heartrateoxygen> heartrateoxygen=heartrateOxygenMappper.listBySessionId(theSessionId);
        data.put("heartrateoxygen",heartrateoxygen);

        //5.获得图片的详细资料
        List<Image> images=photoMapper.findBySessionId(theSessionId);
        data.put("images",images);
        return Result.ok("get all the details of the session_id",data);
    }

    //根据session_id返回图片列表
    private final String baseDirectory="static/imageUploads";
    @Override
    public List<Resource> getImageById(int id) {
        //根据id获得对应的sessionId
        String theSessionId=sessionMapper.getSessionIdById(id);
        System.out.println("theSessionId:"+theSessionId);
        List<Resource> resources=new ArrayList<>();
        Path sessionDirectory= Paths.get(baseDirectory,theSessionId).normalize();

        if(Files.exists(sessionDirectory)&&Files.isDirectory(sessionDirectory)){
            try (DirectoryStream<Path> stream=Files.newDirectoryStream(sessionDirectory,"*{jpg,png,jpeg}")){
                for(Path path:stream){
                    UrlResource resource=new UrlResource(path.toUri());
                    resources.add(resource);

                }
            }catch (IOException|IllegalArgumentException e){
                System.out.println("Error:"+e.getMessage());
            }
        }
        return resources;
    }

    @Override
    public List<Image> getImageDetail(int id) {
        String theSessionId=sessionMapper.getSessionIdById(id);
        System.out.println("theSessionId:"+theSessionId);
        List<Image> images=new ArrayList<>();
        images=photoMapper.findBySessionId(theSessionId);
        return images;
    }

    //新建task_id，同时存入数据库
    @Override
    public boolean setTaksMsg(int rank) {
        //生成task_id
        int task_type=1;//1表示普通训练模式
        long timestamp=System.currentTimeMillis();
        System.out.println("timestamp:"+timestamp);
        String taskId="task_"+timestamp;
        System.out.println("taskId:"+taskId);
        if(rank==0)
        {
            task_type=0;//0表示自定义训练模式
        }
        //插入数据库
        Task task=new Task();
        task.setTaskId(taskId);
        task.setCreatedAt(new Date());
        task.setTaskType(task_type);
        System.out.println("打印出现在的时间"+new Date());
        taskMapper.insert(task);
        return true;
//        return false;
    }

    @Override
    public List<Task> getAllTasks() {
        return taskMapper.selectList(null);
    }
    //通过task_id获得对应所有session_ids
    @Override
    public List<String> getSessionByTask(String task_id) {
        List<String> sessions=sessionMapper.getSessionIdsByTaskId(task_id);
        return sessions;
//        return
//        return List.of();
    }
    //通过sessionId获得对应所有的图片列表
    @Override
    public List<Image> getImageBySession(String session_id) {
        List<Image> imagelist=new ArrayList<>();
        imagelist=imageMapper.getImagesBySessionId(session_id);
        return imagelist;
    }

    //根据session_id获得所有心率血氧数据
    @Override
    public List<Heartrateoxygen> getHeartrateoxygen(String session_id) {
        return heartrateOxygenMappper.listBySessionId(session_id);
    }

    @Override
    public List<Session> getSessionsByTaskId(String task_id) {
        return sessionMapper.getSessionsByTaskId(task_id);
    }

    @Override
    public Typestat calculateSumBySessionIds(List<String> sessionIds) {
        System.out.println("开始聚合查询");
        return typestatMapper.sumTypestatBySessionIds(sessionIds);
    }
    //更新现在的now表,更新当前的训练文本号
    @Override
    public boolean setNowRank(int rank) {
        nowMapper.updateRank(rank);
        return true;
    }

    @Override
    public Typestat getTypestatBySessionId(String session_id) {
        System.out.println("查询问题类型的sessionID"+session_id);
        return typestatMapper.getTypestatBySessionId(session_id);
    }


}
