package com.skelig.demo.mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Update;
import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.skelig.demo.entity.Now;
import org.apache.ibatis.annotations.Select;

public interface NowMapper extends BaseMapper<Now> {
    @Select("SELECT rank_now FROM now WHERE id=#{id}")
    int getRankNow(int id);
    @Select("SELECT sessionid_now FROM now WHERE id=#{id}")
    String getSessionNow(int id);
    //更新rank值
    @Update("UPDATE now SET rank_now = #{rank} WHERE id = 1")
    void updateRank(@Param("rank") int rank);
    @Update("UPDATE now SET sessionid_now = #{sessionId} WHERE id = 1")
    void updateSessionId(@Param("sessionId") String sessionId);
}
