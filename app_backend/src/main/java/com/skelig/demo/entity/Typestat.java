package com.skelig.demo.entity;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
//import lombok.Value;

@AllArgsConstructor
@NoArgsConstructor
@Data
public class Typestat {
    int id;
    int smooth;
    int interrupt;
    int repeatA;
    int repeatB;
    int repeatC;
    int toolong;
    String sessionId;
}
