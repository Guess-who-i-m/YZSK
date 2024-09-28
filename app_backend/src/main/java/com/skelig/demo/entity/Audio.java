package com.skelig.demo.entity;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class Audio {
    private int id;
    private String audioUrl;
    private String sessionId;
}
