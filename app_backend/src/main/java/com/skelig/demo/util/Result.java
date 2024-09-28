package com.skelig.demo.util;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class Result<T> {
    private int status;
    private String message;
    private T data;//使用泛型数据
    //需要提供数据时
    //成功时返回
    public static <T>Result<T> ok(String message,T data) {
        return new Result<>(200, message,data);
    }
    //失败时返回
    public static <T>Result<T> fail(String message,T data) {
        return new Result<>(500, message,data);
    }
    //不需要返回数据时
    public static <T>Result<T> success(String message) {
        return new Result<>(200,message,null);
    }
    public static <T>Result<T> error(String message) {
        return new Result<>(500, message,null);
    }
}
