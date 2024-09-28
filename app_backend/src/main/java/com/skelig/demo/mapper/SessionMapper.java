package com.skelig.demo.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.skelig.demo.entity.Session;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;
import org.apache.ibatis.annotations.Update;

import java.util.List;

public interface SessionMapper extends BaseMapper<Session> {
    @Select("SELECT session_id FROM session WHERE id = #{id}")
    String getSessionIdById(int id);
    @Select("SELECT session_id FROM session WHERE task_id = #{taskId}")
    List<String> getSessionIdsByTaskId(String taskId);
    @Select("SELECT * FROM session WHERE task_id = #{taskId}")
    List<Session> getSessionsByTaskId(String taskId);
    @Select("SELECT * FROM session WHERE session_id = #{sessionId}")
    Session getSessionBySessionId(String sessionId);
    @Select("SELECT task_id FROM session WHERE session_id = #{sessionId}")
    List<String> getTaskidBySessionId(String sessionId);
    @Update("UPDATE session SET score = #{scoreValue} WHERE session_id = #{sessionId}")
    int updateScoreBySessionId(@Param("sessionId") String sessionId, @Param("scoreValue") int scoreValue);
}
