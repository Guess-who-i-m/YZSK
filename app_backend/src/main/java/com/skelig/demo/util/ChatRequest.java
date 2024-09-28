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
public class ChatRequest {
    private String model;
    private Input input;
    private Parameters parameters;

    public ChatRequest(String q){
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
            this.messages.add(new Chat("user", "我是一名口吃患者，这是我在训练过程中的口吃问题和心率以及血氧浓度数据，忽略一些异常数据，请给针对口吃的问题和心率的变化进行分析评价，然后给我一些针对性的建议,包括训练方法，如何进行呼吸调整等"+q));
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
