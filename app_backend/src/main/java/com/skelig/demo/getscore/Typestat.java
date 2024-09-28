package com.skelig.demo.getscore;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@AllArgsConstructor
@Data
@NoArgsConstructor
public class Typestat {
    private int smooth;
    private int interrupt;
    private int repeatA;
    private int repeatB;
    private int repeatC;
    private int toolong;
}
