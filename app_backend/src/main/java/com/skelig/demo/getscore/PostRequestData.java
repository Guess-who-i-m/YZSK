package com.skelig.demo.getscore;

import com.fasterxml.jackson.databind.ObjectMapper;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.MediaType;
import org.springframework.web.client.RestTemplate;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.List;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class PostRequestData {
    private Typestat typestat;
    private List<Hodata> hodata;
    private List<ImageData> imageData;

    private static final Logger logger = LoggerFactory.getLogger(PostRequestData.class);

    public double sendPostRequest(String url, RestTemplate restTemplate) {
        try {
            // 设置请求头
            HttpHeaders headers = new HttpHeaders();
            headers.setContentType(MediaType.APPLICATION_JSON);

            // 创建 HttpEntity
            HttpEntity<PostRequestData> request = new HttpEntity<>(this, headers);

            // 发送 POST 请求并获取响应
            String response = restTemplate.postForObject(url, request, String.class);

            logger.info("Received response: " + response);

            // 使用 Jackson 解析响应
            ObjectMapper objectMapper = new ObjectMapper();
            TotalScoreResponse totalScoreResponse = objectMapper.readValue(response, TotalScoreResponse.class);

            // 返回 total_score
            return totalScoreResponse.getTotal_score();
        } catch (Exception e) {
            logger.error("Failed to send POST request and parse response", e);
            throw new RuntimeException("Failed to send POST request and parse response", e);
        }
    }
}
