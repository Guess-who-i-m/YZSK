package com.skelig.demo.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.skelig.demo.entity.Typestat;
import org.apache.ibatis.annotations.Select;

import java.util.List;

public interface TypestatMapper extends BaseMapper<Typestat> {
    @Select("<script>" +
            "SELECT " +
            "   SUM(smooth) AS smooth, " +
            "   SUM(interrupt) AS interrupt, " +
            "   SUM(repeat_a) AS repeatA, " +
            "   SUM(repeat_b) AS repeatB, " +
            "   SUM(repeat_c) AS repeatC, " +
            "   SUM(toolong) AS toolong " +
            "FROM " +
            "   typestat " +
            "WHERE " +
            "   session_id IN " +
            "<foreach item='sessionId' collection='sessionIds' open='(' separator=',' close=')'>" +
            "   #{sessionId, jdbcType=VARCHAR}" +
            "</foreach>" +
            "</script>")
    Typestat sumTypestatBySessionIds(List<String> sessionIds);
    @Select("SELECT * FROM typestat WHERE session_id = #{sessionId}")
    Typestat getTypestatBySessionId(String sessionId);
}
