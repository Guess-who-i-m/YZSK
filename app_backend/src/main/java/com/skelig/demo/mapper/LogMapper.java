package com.skelig.demo.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.skelig.demo.entity.Log;
import org.apache.ibatis.annotations.Select;
import org.apache.ibatis.annotations.Update;

public interface LogMapper extends BaseMapper<Log> {
    @Select("SELECT score, advice, text_id, text_type FROM log WHERE session_id = #{sessionId}")
    Log getLogDetailsBySessionId(String sessionId);
    @Update("UPDATE log SET advice = #{advice} WHERE session_id = #{sessionId}")
    int updateAdviceBySessionId(String sessionId, String advice);
}
