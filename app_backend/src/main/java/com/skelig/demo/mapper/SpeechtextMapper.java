package com.skelig.demo.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.skelig.demo.entity.Speechtext;
import org.apache.ibatis.annotations.Select;
public interface SpeechtextMapper extends BaseMapper<Speechtext> {
    @Select("SELECT text FROM speechtext WHERE id = #{id}")
    String getTextById(int id);
}
