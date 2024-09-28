package com.skelig.demo.util;
import org.codehaus.jettison.json.JSONObject;
import org.springframework.core.io.FileSystemResource;
import org.springframework.core.io.Resource;
import org.springframework.core.io.UrlResource;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.util.LinkedMultiValueMap;
import org.springframework.util.MultiValueMap;
import org.springframework.web.client.HttpClientErrorException;
import org.springframework.web.client.RestTemplate;
import org.springframework.web.multipart.MultipartFile;
//import org.json.JSONObject;
import org.springframework.stereotype.Service;

import java.io.File;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;

//@Service
public class AskForAdvice {

    public String uploadFileAndParseResponse(MultipartFile file) {
        File tempFile = null;
        try {
            RestTemplate restTemplate = new RestTemplate();
            HttpHeaders headers = new HttpHeaders();
            headers.setContentType(MediaType.MULTIPART_FORM_DATA);

            MultiValueMap<String, Object> body = new LinkedMultiValueMap<>();
            tempFile = Files.createTempFile("upload-", ".tmp").toFile();
            file.transferTo(tempFile);
            body.add("audio_file", new FileSystemResource(tempFile));

            HttpEntity<MultiValueMap<String, Object>> requestEntity = new HttpEntity<>(body, headers);
            String serverUrl = "https://u349276-a810-a83d2396.westc.gpuhub.com:8443/speech-recognization";
            String serverUrl2="https://u349276-a810-a83d2396.westc.gpuhub.com:8443";
            ResponseEntity<String> response = restTemplate.postForEntity(serverUrl, requestEntity, String.class);
            if (!response.getStatusCode().is2xxSuccessful()) {
                return null;  // Early exit if response is not 2xx
            }

            JSONObject jsonResponse = new JSONObject(response.getBody());
            String jsonUrl=jsonResponse.getString("url");
            String finalUrl=serverUrl2+jsonUrl;
            System.out.println("final url is "+finalUrl);
            URL url = new URL(finalUrl);
            Resource resource = new UrlResource(finalUrl);
            System.out.println("Current working directory: " + System.getProperty("user.dir"));
            Path downloadPath = Paths.get("./static/download/downloaded_file.mp3");
            Files.copy(resource.getInputStream(), downloadPath, StandardCopyOption.REPLACE_EXISTING);

            if (jsonResponse.getInt("code") == 200) {
                return jsonResponse.getString("message");
            } else {
                return null;
            }
        } catch (HttpClientErrorException e) {
            e.printStackTrace();
            return null;  // Specific HTTP client errors
        } catch (Exception e) {
            e.printStackTrace();
            return null;  // Other errors
        } finally {
            if (tempFile != null && tempFile.exists()) {
                tempFile.delete();  // Ensure temp file is deleted even if an exception occurs
            }
        }
    }
}
