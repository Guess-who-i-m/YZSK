package com.skelig.demo.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.skelig.demo.entity.Text;
import org.apache.ibatis.annotations.Select;

public interface TextMapper extends BaseMapper<Text> {
    @Select("SELECT text_content, text_type FROM text WHERE id = #{id}")
    Text getTextById(int id);
}
