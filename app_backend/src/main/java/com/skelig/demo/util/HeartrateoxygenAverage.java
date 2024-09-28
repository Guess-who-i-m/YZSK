package com.skelig.demo.util;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class HeartrateoxygenAverage {
    private double heartRateAverage;
    private double oxygenLevelAverage;
}
