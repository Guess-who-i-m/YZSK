package com.skelig.demo.controller;

import com.alibaba.dashscope.exception.InputRequiredException;
import com.alibaba.dashscope.exception.NoApiKeyException;
import com.aliyun.oss.common.comm.ResponseMessage;
import com.skelig.demo.entity.*;
import com.skelig.demo.service.UserService;
import com.skelig.demo.util.HeartrateoxygenAverage;
import com.skelig.demo.util.Result;
import com.skelig.demo.util.Txqw;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.io.Resource;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.servlet.resource.ResourceUrlProvider;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@CrossOrigin
@RestController
public class UserController {
    @Autowired
    private UserService userService;
    @Autowired
    private ResourceUrlProvider mvcResourceUrlProvider;

    @RequestMapping("/user/findAll")
    public List<User> findAll() {
        return userService.searchAllUser();
    }
    //1.获取所有的得分记录，已经弃用
    @GetMapping("/user/getScores")
    public ResponseEntity<Result<?>> getScores() {
        System.out.println("get req of getScores");
        Result result = new Result();
            try {
                result=userService.getScoreCharts();
                return ResponseEntity.ok(result);
            }
            catch (Exception e) {
                e.printStackTrace();
                return ResponseEntity.badRequest().body(result.error("something went wrong"));
            }
    }
    //2.获得所有的基础训练文本
    @GetMapping("/user/getBasicText")
    public ResponseEntity<Result<?>> getBasicText() {
//        System.out.println("get req of getBasicText");
//        System.out.println("req of getBasicText");
        Result result = new Result();
        List<Text> datalist=new ArrayList<>();
        datalist=userService.generateBasicTexts();
        if(datalist.size()>0) {
            Map<String,List<?>> data=new HashMap<>();
            data.put("basictextlist",datalist);
            result=result.ok("获取基础训练文本成功！",data);
            return ResponseEntity.ok(result);
        }
        else {
            return ResponseEntity.badRequest().body(result.error("something went wrong"));
        }
    }
    //3.上传演讲文本，已经弃用
    @PostMapping("/user/uploadSpeechText")
    public ResponseEntity<Result> uploadSpeechText(@RequestParam("speechtext") String speechtext) {
        System.out.println("get req of getSpeechText");
        Result result = new Result();
        if(speechtext!=null) {
            if(userService.processSpeechText(speechtext))
            {
                result=result.success("upload speechtext successfully!");
                return ResponseEntity.ok(result);
            }
            else {
                return ResponseEntity.badRequest().body(result.error("something went wrong"));
            }
        }
        else {
            return ResponseEntity.badRequest().body(result.error("the speech text is null"));
        }
//        return ResponseEntity.badRequest().body(result.error("something went wrong"));
    }
    //4.获取所有的演讲文本，已经弃用
    @GetMapping("/user/getSpeechText")
    public ResponseEntity<Result<?>> getSpeechText() {
        System.out.println("get req of getSpeechText");
        System.out.println("req of getSpeechText");
        Result result = new Result();
        List<Speechtext> datalist=new ArrayList<>();
        datalist=userService.generateSpeechTexts();
        if(datalist.size()>0) {
            Map<String,List<?>> data=new HashMap<>();
            data.put("basictextlist",datalist);
            result=result.ok("获取基础训练文本成功！",data);
            return ResponseEntity.ok(result);
        }
        else {
            return ResponseEntity.badRequest().body(result.error("something went wrong"));
        }
    }
    //5.获取每次训练对应的image信息
//    @GetMapping("/user/images/{id}")
//    public ResponseEntity<Result<?>> getImagesById(@PathVariable int id) {
//        System.out.println("req of getImagesById");
//        List<Resource> images=userService.getImageById(id);
//        System.out.println("images is "+images);
//        System.out.println("images size is "+images.size());
////        images=
//        if(images.size()>0) {
//            Map<String,List<?>> data=new HashMap<>();
//            List<Image> imageList=new ArrayList<>();
//            imageList=userService.getImageDetail(id);
//            data.put("imageList",imageList);
//            data.put("images",images);
//            return ResponseEntity.ok(Result.ok("get images by id successfully!",data));
////          return ResponseEntity.ok(images);
//        }
//        return ResponseEntity.status(404).body(Result.error("there is no image for this id!"));
//    }

    //6.得到具体的信息，已经弃用
    @GetMapping("/user/getDetails/{id}")
    public ResponseEntity<Result<?>> getDetails(@PathVariable int id) {
        System.out.println("req of getDetailsById");
//        Result result = new Result();
        try {
            Result<?> result=userService.generateDetail(id);
            return ResponseEntity.ok(result);
        }catch (Exception e){
            e.printStackTrace();
            return ResponseEntity.notFound().build();
        }
    }
    //7.用户手机端创建一次任务,创建task_id
//    @GetMapping("/user/setTask")
//    public ResponseEntity<Result<?>> setTask() {
//        System.out.println("get req of setTask");
//        Result result = new Result();
//        //调用service函数，生成task_id,时间戳
//        if(userService.setTaksMsg(rank))
//        {
//            return ResponseEntity.ok(result.success("set a task successfully!"));
//        }
//        else {
//            return ResponseEntity.badRequest().body(result.error("set a task failed!"));
//        }
////        return  null;
//    }

    //8.用户查询所有的任务记录
    @GetMapping("/user/getTasks")
    public ResponseEntity<Result<?>> getTasks() {
        Result result = new Result();
        List<Task> tasks=new ArrayList<>();
        tasks=userService.getAllTasks();
        if(tasks.size()>0) {
            Map<String,List<?>> data=new HashMap<>();
            data.put("tasklist",tasks);
            return ResponseEntity.ok(result.ok("get tasks successfully!",data));
        }
        return ResponseEntity.badRequest().body(result.error("get tasks failed!"));
    }

    //9.用户根据task_id获取这个任务内所有的训练记录
    @GetMapping("/user/getSessionMsg/{task_id}")
    public ResponseEntity<Result<?>> getSessionMsg(@PathVariable String task_id) {
        Result result = new Result();
        Map<String,List<?>> data=new HashMap<>();
        //根据task_id获取对应的session_id list
        List<String> sessionIds=userService.getSessionByTask(task_id);
        if(sessionIds.size()>0) {
            System.out.println("sessionIds is "+sessionIds);
            //根据sessionId获取对应的图片信息list
            List<Image> images=new ArrayList<>();
            //这个数组用来存储每次session对应的所有图片列表
            List<List<Image>> listOfImages=new ArrayList<>();
            for(int i=0;i<sessionIds.size();i++) {
                images=userService.getImageBySession(sessionIds.get(i));
                listOfImages.add(images);
//            data.put("images",images);
            }
            //添加到响应数据中
            data.put("listofimages",listOfImages);

            //根据sesisonId获取所有的心率和血氧信息
            List<Heartrateoxygen> heartrateoxygens=new ArrayList<>();
            List<List<Heartrateoxygen>> listOfHeartrateoxygens=new ArrayList<>();
            for (int i=0;i<sessionIds.size();i++) {
                heartrateoxygens=userService.getHeartrateoxygen(sessionIds.get(i));
                listOfHeartrateoxygens.add(heartrateoxygens);
            }
            data.put("listofheartrateoxygens",listOfHeartrateoxygens);

            //根据sessionId获取对应所有得分

            return ResponseEntity.ok(result.ok("get all the list of data successfully!",data));
        }
        return ResponseEntity.badRequest().body(result.error("there is no data for this task!"));
        //返回数据
    }

    //10.用户根据task_id获取这个任务对应所有的训练记录，同时渲染得分数据
    @GetMapping("/user/SessionLogs/{task_id}")
    public ResponseEntity<Result<?>> getSessionLogs(@PathVariable String task_id) {
        Result result = new Result();
        Map<String,List<?>> data=new HashMap<>();
        List<Session> sessionList=new ArrayList<>();
        sessionList=userService.getSessionsByTaskId(task_id);
//        List<Score> scoreList=new ArrayList<>();
        if(sessionList.size()>0) {
            data.put("sessionlist",sessionList);
            return ResponseEntity.ok(result.ok("get session logs successfully!",data));
        }
        else {
            return ResponseEntity.ok(result.error("there is no data for this task!"));
        }
    }

    //11.用户根据输入task_id查询所有这次任务所有训练的口吃问题总和
    @GetMapping("/user/Typestat/{task_id}")
//    @GetMapping("/typestat/{task_id}")
    public ResponseEntity<Result<?>> getTypeStat(@PathVariable String task_id) {
        // 使用 HashMap<String, Object> 来存储具体数据
        Map<String, Object> data = new HashMap<>();
        Result result=new Result();
        if(task_id==null)
        {
            return ResponseEntity.ok(result.error("task_id is null!"));
        }
        try {
            // 根据 task_id 获得所有的 session_id
            List<String> sessionIds = userService.getSessionByTask(task_id);
            System.out.println("sessionIds is "+sessionIds);

            // 返回的聚合统计数据
            Typestat typestat = userService.calculateSumBySessionIds(sessionIds);
            System.out.println(typestat);

            // 将统计数据放入 data map 中
            data.put("totalData", typestat); // 更明确的键名称

            // 创建成功的 Result 对象
//            Result<Map<String, Object>> result = new Result<>(data, "Data retrieved successfully", 200);

            // 返回包含 Result 的 ResponseEntity
            return ResponseEntity.ok(result.ok("typestat successfully!",data));
        } catch (Exception e) {
            // 处理异常情况
//            Result<Void> errorResult = new Result<>(null, "Error retrieving data: " + e.getMessage(), 500);
            return ResponseEntity.internalServerError().body(result.error("typestat failed!"));
        }
    }

    //12.用户创建一个新任务，提交rank参数，rank非零认为是基础训练，rank为0认为是自定义训练
    @PostMapping("/user/buildNewTask/{rank}")
    public ResponseEntity<Result<?>> buildNewTask(@PathVariable int rank) {
        System.out.println("get req of setTask and the rank is "+rank);
        Result result = new Result();
        //设置当前文本rank
        //调用service函数，生成task_id,时间戳,同时需要根据练习模式不同设置task表
        boolean a=userService.setTaksMsg(rank);
        boolean b=userService.setNowRank(rank);
        if(a&&b)
        {
            return ResponseEntity.ok(result.success("set a task successfully!"));
        }
        else {
            return ResponseEntity.ok(result.error("set a task failed!"));
        }
    }

    //12.用户通过sessionId获取本次任务的心率血氧、以及typestat
    @GetMapping("/user/getheartoxygen/{sessionId}")
    public ResponseEntity<Result<?>> gethoOfSession(@PathVariable String sessionId){
        Result result = new Result();
        Map<String,Object> data=new HashMap<>();
        List<Heartrateoxygen> heartrateoxygens=userService.getHeartrateoxygen(sessionId);
        data.put("heartrateoxygens",heartrateoxygens);
        Typestat typestat=userService.getTypestatBySessionId(sessionId);
        data.put("typestat",typestat);
        return ResponseEntity.ok(result.ok("typestat successfully!",data));
    }
    //13.发送综合数据，得到建议
    @GetMapping("/user/getGptAdvice")
    public ResponseEntity<Result<?>> getGptAdvice() throws NoApiKeyException, InputRequiredException {
        Result result=new Result();
        Txqw.callWithMessage();
        return ResponseEntity.ok(result.success("ok"));
    }
    //14.根据task_id获取每次训练的平均心率血氧数据
    @GetMapping("/user/getHodataBytaskid/{task_id}")
    public ResponseEntity<Result<?>> getHodataBytaskid(@PathVariable String task_id) {
        Result result = new Result();
        Map<String, List<HeartrateoxygenAverage>> data = new HashMap<>();

        // 获取 session 列表
        List<Session> sessionList = userService.getSessionsByTaskId(task_id);
        System.out.println("sessionList is "+sessionList);
        List<HeartrateoxygenAverage> averages = new ArrayList<>();

        for (Session session : sessionList) {
            // 根据 sessionId 查询获得 Heartrateoxygen 列表
            List<Heartrateoxygen> heartrateoxygens = userService.getHeartrateoxygen(session.getSessionId());
            System.out.println("heartrateoxygens is "+heartrateoxygens);
            // 计算平均值
            double heartRateSum = 0;
            double oxygenLevelSum = 0;
            int count = heartrateoxygens.size();

            for (Heartrateoxygen ho : heartrateoxygens) {
                heartRateSum += ho.getHeartRate();
                oxygenLevelSum += ho.getOxygenLevel();
            }

            double heartRateAverage = count == 0 ? 0 : heartRateSum / count;
            double oxygenLevelAverage = count == 0 ? 0 : oxygenLevelSum / count;

            averages.add(new HeartrateoxygenAverage(heartRateAverage, oxygenLevelAverage));
        }

        data.put("averages", averages);
        result.setData(data);
        return ResponseEntity.ok(result.ok("get average data successfully!",data));
    }

    //15.根据sessionId获取对应图片的信息
    @GetMapping("/user/getImageBySessionId/{sessionId}")
    public ResponseEntity<Result<?>> getImageBySessionId(@PathVariable String sessionId) {
            Result result = new Result();
            Map<String,Object> data=new HashMap<>();
            try {
                List<Image> imagelist=userService.getImageBySession(sessionId);
                data.put("imagelist",imagelist);
                return ResponseEntity.ok(result.ok("image successfully!",data));
            }catch (Exception e) {
                return ResponseEntity.internalServerError().body(result.error("image failed!"));
            }



    }
}














