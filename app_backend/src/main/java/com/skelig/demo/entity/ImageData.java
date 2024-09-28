package com.skelig.demo.entity;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

//接收开发板发来的json格式的数据
@Data
@AllArgsConstructor
@NoArgsConstructor
public class ImageData {
    private String file;
    private String session_id;
    private String encodeType;
}
