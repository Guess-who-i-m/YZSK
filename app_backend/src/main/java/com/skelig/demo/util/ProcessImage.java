package com.skelig.demo.util;

import org.springframework.web.multipart.MultipartFile;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.file.Files;
import java.nio.file.Path;

public class ProcessImage {
    public String getImageData(MultipartFile imagefile) {
        if (imagefile.isEmpty()) {
            return null;
        }
        try {
            // 保存上传的文件到临时目录
            Path tempFile = Files.createTempFile("upload-", imagefile.getOriginalFilename());
            imagefile.transferTo(tempFile.toFile());
            System.out.println("文件路径" + tempFile.toString());

            // 调用Python脚本
            ProcessBuilder pb = new ProcessBuilder("python", "D:\\Python\\esp1\\test.py", tempFile.toString());
            System.out.println("脚本命令：" + String.join(" ", pb.command()));
            pb.redirectErrorStream(true);
            Process process = pb.start();

            // 仅读取最后一行输出
            BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            String lastLine = "";
            String line;
            while ((line = reader.readLine()) != null) {
                lastLine = line;  // 更新为最新的行
            }

            // 删除临时文件
            Files.deleteIfExists(tempFile);

            System.out.println("脚本输出内容如下：" + lastLine);
            return lastLine;  // 返回最后一行的内容
        } catch (IOException e) {
            e.printStackTrace();
            return "Failed to process image.";
        }
    }
}
