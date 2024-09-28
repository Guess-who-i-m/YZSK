package com.skelig.demo.mapper;


import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.skelig.demo.entity.Heartrateoxygen;
import org.apache.ibatis.annotations.Select;

import java.util.List;

public interface HeartrateOxygenMappper extends BaseMapper<Heartrateoxygen> {
    @Select("SELECT * FROM heartrateoxygen WHERE session_id = #{sessionId}")
    List<Heartrateoxygen> listBySessionId(String sessionId);
    @Select("SELECT heart_rate,oxygen_level FROM heartrateoxygen WHERE session_id = #{sessionId}")
    List<Heartrateoxygen> getListBySessionId(String sessionId);
}
