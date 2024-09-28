package com.skelig.demo.util;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.List;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class ChatreqData {
    private int smooth;        // 流畅次数
    private int interrupt;     // 中断次数
    private int repeatA;       // 音节重复次数
    private int repeatB;       // 字重复次数
    private int repeatC;       // 词重复次数
    private int toolong;       // 过分长音次数
    private List<Reading> readings;  // 心率和血氧水平数据列表

    @Data
    @AllArgsConstructor
    @NoArgsConstructor
    public static class Reading {
        private int heartRateAverage;    // 心率
        private double oxygenLevelAverage; // 血氧水平
    }
}
