package com.skelig.demo.entity;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.Date;
@Data
@AllArgsConstructor
@NoArgsConstructor
public class Session {
    private int id;
    private String sessionId;
    private Date createdAt;
    private String taskId;
    private int score;
}
