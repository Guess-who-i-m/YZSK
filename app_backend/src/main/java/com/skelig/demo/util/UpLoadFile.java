package com.skelig.demo.util;

import cn.hutool.core.io.file.FileNameUtil;
import com.aliyun.oss.OSS;
import com.aliyun.oss.OSSClient;
import org.apache.commons.io.FilenameUtils;
import org.springframework.web.multipart.MultipartFile;

import java.io.IOException;
import java.util.UUID;

public class UpLoadFile {
    private static final String ENDPOINT = "http://oss-cn-beijing.aliyuncs.com";
    private static final String ACCESS_KEY_ID = "LTAI5t8US68k8ZSEQD7FHzSd";
    private static final String ACCESS_KEY_SECRET = "EeRQooiBaOTaVHZsqOOjeBtGM7x6xW";
    private static final String BUCKET_NAME = "yanzushengkai";
    private static final String ALI_DOMAIN = "http://yanzushengkai.oss-cn-beijing.aliyuncs.com/";
//    public static final String  ALI_DOMAIN="https://yanzushengkai.oss-cn-beijing.aliyuncs.com/";
    public static String upLoadImage(MultipartFile file) throws IOException {
        //构造文件名
        //原文件名
        String originalFilename = file.getOriginalFilename();
        String ext="."+ FilenameUtils.getExtension(originalFilename);
        String uuid = UUID.randomUUID().toString().replace("-","");
        String fileName=uuid+ext;
        //地域节点
//        String endpoint="http://oss-cn-beijing.aliyuncs.com";
//        String accessKeyId= "LTAI5t8US68k8ZSEQD7FHzSd";
//        String accessKeySecret= "EeRQooiBaOTaVHZsqOOjeBtGM7x6xW";
        //OSS客户端对象
        OSS ossClient = new OSSClient(ENDPOINT,ACCESS_KEY_ID,ACCESS_KEY_SECRET);
        ossClient.putObject(
                BUCKET_NAME,
                fileName,
                file.getInputStream()
        );
        ossClient.shutdown();
        return ALI_DOMAIN+fileName;
//        return null;
    }

    //上传音频文件到云端
    public static String upLoadAudio(MultipartFile file) throws IOException {
        // 构造文件名
        // 原文件名
        String originalFilename = file.getOriginalFilename();
        String ext = "." + FilenameUtils.getExtension(originalFilename);
        String uuid = UUID.randomUUID().toString().replace("-", "");
        String fileName = uuid + ext;

        // OSS客户端对象
        OSS ossClient = new OSSClient(ENDPOINT, ACCESS_KEY_ID, ACCESS_KEY_SECRET);

        // 上传文件
        ossClient.putObject(BUCKET_NAME, fileName, file.getInputStream());

        // 关闭OSS客户端
        ossClient.shutdown();

        // 返回文件的URL
        return ALI_DOMAIN + fileName;
    }


}
