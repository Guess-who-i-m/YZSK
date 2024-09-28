package com.skelig.demo.entity;


import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import org.springframework.beans.factory.annotation.Autowired;

@Data
@AllArgsConstructor
@NoArgsConstructor

//这个用于设置当天用户正在训练的sessionid，以及选择的文本标号，如果用户使用的是
public class Now {
    int id;
    String sessionidNow;
    int rankNow;
}
