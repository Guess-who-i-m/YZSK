package com.skelig.demo.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.skelig.demo.entity.Image;
import org.apache.ibatis.annotations.Select;

import java.util.List;

public interface ImageMapper extends BaseMapper<Image> {
    @Select("SELECT * FROM image WHERE session_id = #{sessionId}")
    List<Image> getImagesBySessionId(String sessionId);
}
