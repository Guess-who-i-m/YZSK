package com.skelig.demo.entity;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.sql.Date;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class Log {
    private int id;
    private String sessionId;
    private int userId;
//    private Date dateTime;
    private int score;
//    private String evaluation;
    private String advice;
    private int textId;
    private int textType;
//    private int basicTextType;
}
