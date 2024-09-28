package com.skelig.demo.entity;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.sql.Date;
@Data
@NoArgsConstructor
@AllArgsConstructor
public class Image {
    private int id;
    private String sessionId;
    private String imageUrl;
//    private Date timestamp;
    private String imageData;
}
