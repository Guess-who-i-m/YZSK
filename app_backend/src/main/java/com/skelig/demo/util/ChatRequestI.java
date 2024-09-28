package com.skelig.demo.util;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.ArrayList;
import java.util.List;

// 处理请求
@Data
@AllArgsConstructor
@NoArgsConstructor
public class ChatRequestI {
    private String model;
    private Input input;
    private Parameters parameters;

    public ChatRequestI(String q){
        this.model = "qwen-turbo";
        this.input = new Input(q);
        this.parameters = new Parameters();
    }

    @Data
    @AllArgsConstructor
    @NoArgsConstructor
    public static class Input {
        private List<Chat> messages;

        public Input(String q){
            this.messages = new ArrayList<>();
            this.messages.add(new Chat("system", "你是一名资深的口吃矫正专家"));
            this.messages.add(new Chat("user", "我是一名口吃患者，我有一些问题"+q));
        }
    }

    @Data
    @AllArgsConstructor
    @NoArgsConstructor
    public static class Chat {
        private String role;
        private String content;
    }

    @Data
    @AllArgsConstructor
    @NoArgsConstructor
    public static class Parameters {
        private String result_format = "text";
    }
}
