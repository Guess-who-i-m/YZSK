package com.skelig.demo.util;

import org.springframework.web.multipart.MultipartFile;

import java.io.IOException;
import java.util.Base64;

public class DecodeBase64 {
    public static MultipartFile decodeBase64ToMultipartFile(String base64EncodedFile, String contentType) {
        try {
            // 去掉Base64数据前的前缀（如果有，如"data:image/jpeg;base64,"）
            if (base64EncodedFile.contains(",")) {
                base64EncodedFile = base64EncodedFile.substring(base64EncodedFile.indexOf(",") + 1);
            }

            // Base64 解码
            byte[] fileBytes = Base64.getDecoder().decode(base64EncodedFile);

            // 将字节数组转换为 MultipartFile
            return new ByteArrayMultipartFile(fileBytes, "filename", "filename." + contentType, "image/" + contentType);
        } catch (IllegalArgumentException e) {
            return null;
        }
    }
}
