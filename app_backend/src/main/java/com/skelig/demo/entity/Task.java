package com.skelig.demo.entity;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.Date;
@Data
@AllArgsConstructor
@NoArgsConstructor
public class Task {
    private String taskId;
    private Date createdAt;
    int id;
    int taskType;
}
