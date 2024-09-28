from flask import Flask, request, send_file, jsonify
from flask_cors import CORS
import uuid
import os

import stable_whisper
from http import HTTPStatus
import dashscope
from dashscope.audio.tts import SpeechSynthesizer

import model.FluentNet as net
import torch
import torch.nn.functional as F
import matplotlib.pyplot as plt


app = Flask(__name__, static_folder='static')
cors = CORS(app)

dashscope.api_key = 'sk-baf93a17a95347288868302988389c7e'

file_dir = "./static"


from pydub import AudioSegment

def stereo_to_mono(input_file, output_dir):
    """
    将双声道音频文件转换为单声道，并每隔4秒进行一次切割，保存到指定文件夹中
    参数:
    input_file (str): 输入音频文件路径
    output_dir (str): 输出文件夹路径
    """
    # 创建输出文件夹，如果不存在则创建
    os.makedirs(output_dir, exist_ok=True)
    
    # 加载音频文件
    audio = AudioSegment.from_file(input_file)
    
    # 转换为单声道
    mono_audio = audio.set_channels(1)
    
    # 获取音频时长（毫秒）
    duration = len(mono_audio)
    
    # 每隔4秒切割音频并保存
    segment_duration = 4 * 1000  # 4秒，单位为毫秒
    for i in range(0, duration, segment_duration):
        segment = mono_audio[i:i + segment_duration]
        segment_name = os.path.join(output_dir, f'segment_{i // segment_duration}.wav')
        segment.export(segment_name, format="wav")
        print(f'Saved {segment_name}')

# 流式调用大模型
def call_with_messages_stream(message):
    messages = [{'role': 'system', 'content': '假设你是一个医生，有一位口吃患者正在接受你的言语治疗，这是他说的话经过语音识别并初步处理得到的内容，start是语言的起始时间，end是语言的结束时间，请你分析语言中字的重复情况和语音的过分拖长情况，并对他的治疗给出一定的建议。注意：语音识别的内容不准确，只参考重复度。生成一段你作为医生对患者说的话。并且你的回复中不要包含分点和换行符。'},
                {'role': 'user', 'content': message}]

    responses = dashscope.Generation.call(
        dashscope.Generation.Models.qwen_max,
        messages=messages,
        result_format='message',  # 将返回结果格式设置为 message
        stream=True,  # 设置输出方式为流式输出
        incremental_output=True  # 增量式流式输出
    )
    for response in responses:
        if response.status_code == HTTPStatus.OK:
            # print(response)s
            print(response.output.choices[0]['message']['content'], end='')
        else:
            print('Request id: %s, Status code: %s, error code: %s, error message: %s' % (
                response.request_id, response.status_code,
                response.code, response.message
            ))
    return responses

# 正常调用大模型
def call_with_messages(message):
    messages = [{'role': 'system', 'content': '假设你是一个医生，有一位口吃患者正在接受你的言语治疗，这是他说的话经过语音识别并初步处理得到的内容，start是语言的起始时间，end是语言的结束时间，请你分析语言中字的重复情况和语音的过分拖长情况，并对他的治疗给出一定的建议。注意：语音识别的内容不准确，只参考重复度。生成一段你作为医生对患者说的话。并且你的回复中不要包含分点和换行符。并且你的回复需要尽可能简洁。'},
                {'role': 'user', 'content': message}]

    response = dashscope.Generation.call(
        dashscope.Generation.Models.qwen_max,
        messages=messages,
        result_format='message',  # 将返回结果格式设置为 message
    )
    if response.status_code == HTTPStatus.OK:
        print(response)
    else:
        print('Request id: %s, Status code: %s, error code: %s, error message: %s' % (
            response.request_id, response.status_code,
            response.code, response.message
        ))
    return response

# 正常进行语音合成
def tts(advice, output_path):
    text = advice.output.choices[0]['message']['content']
    
    result = SpeechSynthesizer.call(model='sambert-zhichu-v1',
                                text=text,
                                sample_rate=48000,
                                format='mp3')

    if result.get_audio_data() is not None:
        with open(output_path, 'wb') as f:
            f.write(result.get_audio_data())
    print('  get response: %s' % (result.get_response()))
    
# 流式合成语音
def tts_stream(advices, output_dir):
    for advice in advices:
        print(advice.output.choices[0]['message']['content'])
        
    i = 0
    for advice in advices:
        text = advice.output.choices[0]['message']['content']
        
        result = SpeechSynthesizer.call(model='sambert-zhichu-v1',
                                    text=text,
                                    sample_rate=48000,
                                    format='mp3')

        if result.get_audio_data() is not None:
            with open(f'{output_dir}/output_{i}.wav', 'wb') as f:
                f.write(result.get_audio_data())
                i += 1
        print('  get response: %s' % (result.get_response()))
    

# 语音识别路由
@app.route('/speech-recognization', methods=['POST'])
def handle_audio():
    
    # 首先在请求内容中找到数据
    if 'audio_file' not in request.files:
        return jsonify({'error':'No file part'}), 400
    
    
    file = request.files['audio_file']
    filename = str(uuid.uuid4()) + ".mp3"
    file_path = os.path.join(file_dir, 'input', filename)
    file.save(file_path)
    
    
    model = stable_whisper.load_model('small', device = 'cuda')
    results = model.transcribe(file_path, language = 'zh')
    
    word_list = []

    for result in results:
        word_list += result.words
        
    message = ""    
    
    for word in word_list:
        message += str(word) + ", "
        # print(word)
        
    print(message)
    # result.to_srt_vtt('audio.srt', segment_level = False, word_level = True) # SRT
    
    advice = call_with_messages(message)

    print(advice.output.choices[0]['message']['content'])
    
    output_path = os.path.join(file_dir, "output", filename)
    
    tts(advice, output_path)
    
    # advices = call_with_messages_stream(message)
    # output_dir = os.path.join(file_dir, "output")
    
    return jsonify({'code':200, 'url': os.path.join("/static/output", filename), 'message': advice.output.choices[0]['message']['content'] })


@app.route('/delete', methods = ['GET'])
def delete_files():
    input_dir = os.path.join(file_dir, 'input')
    output_dir = os.path.join(file_dir, 'output')
    # shutil.rmtree(input_dir)
    # shutil.rmtree(output_dir)

    for file in os.listdir(input_dir):
        file_path = os.path.join(input_dir, file)
        try:
            os.unlink(file_path)
            print(f"已删除 {file_path}")
        except Exception as e:
            print(f"删除 {file_path} 时出错。原因: {e}")
            
    for file in os.listdir(output_dir):
        file_path = os.path.join(output_dir, file)
        try:
            os.unlink(file_path)
            print(f"已删除 {file_path}")
        except Exception as e:
            print(f"删除 {file_path} 时出错。原因: {e}")
            
    return jsonify({'code':200, 'message':"成功删除！"})


# FluentNet模型推理
@app.route('/FluentNet', methods=['POST'])
def inference():
    
    # 首先在请求内容中找到数据
    if 'audio_file' not in request.files:
        return jsonify({'error':'No file part'}), 400
    
    
    file = request.files['audio_file']
    filename = str(uuid.uuid4()) + ".wav"
    file_path = os.path.join(file_dir, 'input', filename)
    file.save(file_path)
    
    input_dir = os.path.join(file_dir, "input")
    output_dir = os.path.join(file_dir, "output")
    
    stereo_to_mono(file_path, output_dir)
    
    
    model = net.FluentNet()
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model.to(device)
    
    # 加载检查点
    checkpoint = torch.load('./checkpoint/checkpoint_3.pth')
    model.load_state_dict(checkpoint['model_state_dict'])
    
    # 设置模型为评估模式
    model.eval()
    
    result = [0, 0, 0, 0, 0, 0]
    
    for filename in os.listdir(output_dir):
        audio = os.path.join(output_dir, filename)
        net.graph_spectrogram(audio)
        img_path = "./static/image.png"
        plt.savefig(img_path)
        plt.close()
        
        input_tensor = net.preprocess(img_path)
        input_batch = input_tensor.unsqueeze(0)
        
        input_batch = input_batch.to(device)
        
        # 进行推理
        print("推理" + filename)
        with torch.no_grad():
            output = model(input_batch)
            output = output[:, 0, :]
            
            # 使用 softmax 将输出转化为概率
            probabilities = F.softmax(output, dim=1)

            # 获取概率最高的类别
            max_prob = probabilities.max(dim=1, keepdim=True)[0]    # 获取最高概率
            all_max_indices = (probabilities == max_prob).nonzero(as_tuple=True)[1]     # 获取最高概率的所有可能
      
        # 打印所有最大值对应的标签
        for index in all_max_indices:
            print(str(index.item()))
            result[index.item()] += 1
        
    for file in os.listdir(input_dir):
        file_path = os.path.join(input_dir, file)
        try:
            os.unlink(file_path)
            print(f"已删除 {file_path}")
        except Exception as e:
            print(f"删除 {file_path} 时出错。原因: {e}")
            
    for file in os.listdir(output_dir):
        file_path = os.path.join(output_dir, file)
        try:
            os.unlink(file_path)
            print(f"已删除 {file_path}")
        except Exception as e:
            print(f"删除 {file_path} 时出错。原因: {e}")
        
        
    return jsonify(result)

# 评分系统
@app.route('/GetScores', methods=['POST'])
def get_scores():
    data = request.json
    
    typestat = data['typestat']
    hodata = data['hodata']
    imageData = data['imageData']
    
    audio_score = calculate_audio_score(typestat)
    heart_oxygen_score = calculate_heart_oxygen_score(hodata, typestat['smooth'])
    image_data_score = calculate_image_data_score(imageData, typestat)
    
    weighted_audio_score = audio_score * 0.6
    weighted_heart_oxygen_score = heart_oxygen_score * 0.2
    weighted_image_data_score = image_data_score * 0.2
    
    raw_total_score = weighted_audio_score + weighted_heart_oxygen_score + weighted_image_data_score
    
    # 假设最大可能得分和最小可能得分
    max_score = 100
    min_score = -100
    
    # 归一化到0到100的区间
    normalized_score = (raw_total_score - min_score) / (max_score - min_score) * 100
    
    response = {
        'total_score': normalized_score
    }
    
    return jsonify(response)

def calculate_audio_score(typestat):
    scores = {
        'smooth': 5,
        'interrupt': -5,
        'repeatA': -3,
        'repeatB': -3,
        'repeatC': -3,
        'toolong': -4
    }
    
    total_score = 0
    for key, value in typestat.items():
        total_score += scores.get(key, 0) * value
    return total_score

def calculate_heart_oxygen_score(hodata, smooth):
    heart_rate_score = 0
    oxygen_level_score = 0
    
    for data in hodata:
        heart_rate = data['heartRate']
        oxygen_level = data['oxygenLevel']
        
        if heart_rate < 60:
            heart_rate_score -= (60 - heart_rate) * 0.5
        elif heart_rate > 100:
            heart_rate_score -= (heart_rate - 100) * 0.5
        
        if oxygen_level < 95:
            oxygen_level_score -= (95 - oxygen_level) * 2
    
    # 根据smooth调整心率和血氧评分
    adjustment_factor = 1 - smooth * 0.05
    heart_rate_score *= adjustment_factor
    oxygen_level_score *= adjustment_factor
    
    return heart_rate_score + oxygen_level_score

def calculate_image_data_score(imageData, typestat):
    scores = {
        'Happy': 3,
        'Angry': -2,
        'Disgust': -2,
        'Fear': -2,
        'Sad': -2,
        'Surprise': 0,
        'Neutral': 0
    }
    
    total_score = 0
    for data in imageData:
        emotion = data['imageData']
        total_score += scores.get(emotion, 0)
    
    # 根据typestat调整面部表情评分
    negative_speech_count = typestat['interrupt'] + typestat['repeatA'] + typestat['repeatB'] + typestat['repeatC'] + typestat['toolong']
    adjustment_factor = 1 - negative_speech_count * 0.02
    total_score *= adjustment_factor
    
    return total_score

# 运行启动后端
if __name__ == '__main__':
    
    app.run(debug=True, port = 6006)
    
    
    