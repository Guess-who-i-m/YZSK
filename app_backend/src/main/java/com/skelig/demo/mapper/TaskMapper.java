package com.skelig.demo.mapper;

//import cn.hutool.cron.task.Task;
import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.skelig.demo.entity.Task;
import org.apache.ibatis.annotations.Select;

public interface TaskMapper extends BaseMapper<Task> {
    @Select("SELECT task_id FROM task ORDER BY id DESC LIMIT 1")
    String getLastTaskId();
    @Select("SELECT task_type FROM task WHERE task_id = #{taskId}")
    int getTaskTypeByTaskId(String taskId);
}
