package com.skelig.demo.service;

import com.skelig.demo.entity.*;
import com.skelig.demo.util.Result;
import org.springframework.core.io.Resource;

import java.util.List;

public interface UserService {
    List<User> searchAllUser();
    Result<?> getScoreCharts();
    List<Text> generateBasicTexts();
//    List<Text> generateSpeechTexts();
    boolean processSpeechText(String text);
    List<Speechtext> generateSpeechTexts();
    Result<?> generateDetail(int id);
    List<Resource> getImageById(int id);
    List<Image> getImageDetail(int id);
    //生成task_id，时间戳，存入数据库
    boolean setTaksMsg(int rank);
    List<Task> getAllTasks();
    List<String> getSessionByTask(String task_id);
    List<Image> getImageBySession(String session_id);
    List<Heartrateoxygen> getHeartrateoxygen(String session_id);
    List<Session> getSessionsByTaskId(String task_id);
//    List<Score> getScoreBySessionId(String session_id);
    Typestat calculateSumBySessionIds(List<String> sessionIds);
    //更新now表中的文本号
    boolean setNowRank(int rank);
    Typestat getTypestatBySessionId(String session_id);
}
