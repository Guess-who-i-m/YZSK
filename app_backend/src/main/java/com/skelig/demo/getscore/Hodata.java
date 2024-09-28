package com.skelig.demo.getscore;


import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.Date;

@AllArgsConstructor
@Data
@NoArgsConstructor
public class Hodata {
    private int heartRate;
    private double oxygenLevel;
    private Date recordTime;
}
