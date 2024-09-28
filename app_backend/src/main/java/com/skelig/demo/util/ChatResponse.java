package com.skelig.demo.util;


import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class ChatResponse {
    Output output;
    Usage usage;
    String request_id;
    class Output{
        public String text;
        public String finish_reason;
    }
    class Usage{
        public String output_tokens;
        public String input_tokens;
    }
}
