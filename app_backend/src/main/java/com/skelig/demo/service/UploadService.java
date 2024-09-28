package com.skelig.demo.service;

import com.skelig.demo.entity.Image;
import com.skelig.demo.mapper.AudioMapper;
import com.skelig.demo.mapper.TypestatMapper;
import com.skelig.demo.util.Result;
import org.springframework.web.multipart.MultipartFile;

import java.util.List;

public interface UploadService {
  String processAudio(MultipartFile audiofile,String sessionId);//处理音频文件
  Result<?> getTrainingText(int rank);//返回训练文本
  Result<?> getSpeechText(int rank);
  boolean processImage(MultipartFile imagefile,String session_id);//处理图片
  void processHeartrateOxygen(int heartrate,double oxygen,String sessionId);//处理血氧心率
  String generateAdvice(MultipartFile audiofile,String sessionId);
  //生成sessionId，now表中更新sessionId，更新session表
  String getSessionId();
  String getNowText();
//  void AudioProcessor(AudioMapper audioMapper, TypestatMapper typestatMapper)
}
