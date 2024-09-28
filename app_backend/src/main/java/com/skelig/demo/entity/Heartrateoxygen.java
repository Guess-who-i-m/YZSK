package com.skelig.demo.entity;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.Date;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class Heartrateoxygen {
    private int id;
    private String sessionId;
    private int heartRate;
    private Double oxygenLevel;
    private Date recordTime;
}
