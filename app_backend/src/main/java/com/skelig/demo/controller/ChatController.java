package com.skelig.demo.controller;

import cn.hutool.http.HttpRequest;
import cn.hutool.json.JSONUtil;
import com.skelig.demo.util.ChatRequest;
import com.skelig.demo.util.ChatRequestI;
import com.skelig.demo.util.ChatResponse;
import com.skelig.demo.util.ChatreqData;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.web.bind.annotation.*;

import java.util.Random;

@CrossOrigin
@RestController
public class ChatController {

    @Value("${api.url}")
    private String url;

    @Value("${api.key}")
    private String apiKey;

    @PostMapping("/chat")
    public ChatResponse chat(@RequestBody ChatreqData chatreqData) {
        // 将ChatreqData对象转换为模型能够处理的输入格式
        String q = processChatreqData(chatreqData);
        System.out.println("输入的字符串q"+q);

        // 创建ChatRequest对象，包含预定义的要求和用户问题
//        String test="你叫什么";
        ChatRequest chatRequest = new ChatRequest(q);

        // 转换为JSON字符串
        String json = JSONUtil.toJsonStr(chatRequest);
        System.out.println("json转化为的字符串"+json);
        String result;
        System.out.println("aipkey"+apiKey);
        try {
            result = HttpRequest.post(url)
                    .header("Authorization", "Bearer " + apiKey)
                    .header("Content-Type", "application/json")
                    .body(json)
                    .execute().body();
        } catch (Exception e) {
            // 添加错误处理
            return new ChatResponse(null, null, "Request failed: " + e.getMessage());
        }
        System.out.println("result"+result);
        return JSONUtil.toBean(result, ChatResponse.class);
    }

    @GetMapping("/getNowAdvice")
    public ChatResponse getNowAdvice() {
        // 定义问题字符串
        String q1 = "我在进行言语训练时无法流畅读出训练内容，常常卡在某几个字无法发音，同时我感到紧张和着急,请你给我50字左右的简要发音指导建议";
        String q2 = "我在进行言语训练时可以流畅的读出一小部分内容，但是在读一些字的时候会无法发出全音，会卡在一些音节上，同时我感到紧张和着急,请你给我50字左右的简要发音指导建议";
        String q3 = "我在进行言语训练时很难完成整个训练，常常会卡在某个字上，我会感到着急和紧张，但是越紧张越无法流畅发音，请你给我50字左右的简要发音指导建议";

        // 生成随机数来选择问题
        int min = 1;
        int max = 3;
        Random random = new Random();
        int randomNum = random.nextInt(max - min + 1) + min;

        // 根据随机数选择问题
        String selectedQuestion;
        if (randomNum == 1) {
            selectedQuestion = q1;
        } else if (randomNum == 2) {
            selectedQuestion = q2;
        } else {
            selectedQuestion = q3;
        }

        // 创建 ChatRequestI 对象
        ChatRequestI chatRequestI = new ChatRequestI(selectedQuestion);
        String json = JSONUtil.toJsonStr(chatRequestI);
        System.out.println("json转化为：" + json);

        String result;
        try {
            // 发送 HTTP 请求
            result = HttpRequest.post(url)
                    .header("Authorization", "Bearer " + apiKey)
                    .header("Content-Type", "application/json")
                    .body(json)
                    .execute().body();
        } catch (Exception e) {
            // 添加错误处理
            return new ChatResponse(null, null, "Request failed: " + e.getMessage());
        }

        System.out.println("result" + result);

        // 清除 Markdown 并返回结果
        String text = cleanMarkdown(result);
//        text=text+"this is the content just for test";
        System.out.println("the text is "+text);
        return JSONUtil.toBean(text, ChatResponse.class);
    }

    public static String cleanMarkdown(String text) {
        // Remove markdown symbols for headers, bold, and lists
        text = text.replaceAll("#+", "");  // Remove header symbols (###)
        text = text.replaceAll("\\*\\*", "");  // Remove bold symbols (**)
        text = text.replaceAll("\\*", "");  // Remove asterisks (*)
        text = text.replaceAll("\\n+", " ");  // Remove newline characters (\n) and replace with space
        text = text.replaceAll("\\d+\\.", ""); // Remove list item numbers (1., 2., etc.)
        text = text.replaceAll("-", ""); // Remove list item dashes (-)

        // Trim leading/trailing whitespace
        return text.trim();
    }
    private String processChatreqData(ChatreqData chatreqData) {
        // 根据ChatreqData对象的内容生成字符串
        StringBuilder sb = new StringBuilder();
        sb.append("用户朗读数据统计如下：\n");
        sb.append("流畅次数: ").append(chatreqData.getSmooth()).append("\n");
        sb.append("中断次数: ").append(chatreqData.getInterrupt()).append("\n");
        sb.append("音节重复: ").append(chatreqData.getRepeatA()).append("\n");
        sb.append("字重复: ").append(chatreqData.getRepeatB()).append("\n");
        sb.append("词重复: ").append(chatreqData.getRepeatC()).append("\n");
        sb.append("过分长音次数: ").append(chatreqData.getToolong()).append("\n");
        sb.append("每次训练的平均心率和血氧水平数据如下：\n");

        int trainingSession = 1;
        for (ChatreqData.Reading reading : chatreqData.getReadings()) {
            sb.append("训练 ").append(trainingSession++).append(": ");
            sb.append("心率: ").append(reading.getHeartRateAverage()).append(", ");
            sb.append("血氧水平: ").append(reading.getOxygenLevelAverage()).append("\n");
        }

        return sb.toString();
    }
}
