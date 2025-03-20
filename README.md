# YZSK

 言阻声开——基于ESP32S3Box3的智能口吃矫正助手

## 前端文档

此部分简单介绍前端的主要结构和业务逻辑。

- 前端基于 Vue3 + Vite 构建，主要组件如下：
  - App.vue：应用入口
  - router/index.js：页面路由
  - components：基础组件与功能封装
- 功能概览：
  - 任务管理页面、会话数据可视化、音频/图片上传与实时监控
  - 与后端 API 交互获取训练文本、任务列表、统计数据等
- 页面结构说明：
  1. 任务管理页面（/tasks）
     - 展示所有任务列表，对应后端API：GET /user/getTasks
     - 支持查看任务详情，对应后端API：GET /user/getSessionMsg/{task_id}
  2. 训练数据页面（/session）
     - 展示每次训练的详细数据，如图片、心率血氧，对应后端API：GET /user/getSessionMsg/{task_id}
     - 数据可视化，调用后端统计接口：GET /user/getHodataBytaskid/{task_id}
  3. 口吃统计页面（/stats）
     - 展示口吃类型统计与分析结果，对应后端API：GET /user/Typestat/{task_id}
  4. 训练文本与演讲页面（/texts）
     - 获取训练文本或演讲内容，对应后端API：GET /speech/text/{rank}
     - （已弃用）GET /training/text/{rank}
  5. Chat 页面（/chat）
     - 与后端进行实时对话，调用接口：POST /chat
     - 获取推荐发音指导建议：GET /getNowAdvice

## 后端文档

该文档简要列举了项目中常用的后端接口，方便快速了解各功能的调用方式和数据结构。以下链接中提到的源码均可在 [app_backend/src/main/java/com/skelig/demo](app_backend/src/main/java/com/skelig/demo) 路径下找到。

---

### BoardController

位于 [BoardController.java](app_backend/src/main/java/com/skelig/demo/controller/BoardController.java)，主要负责音频、图片、心率血氧等数据的上传和处理。

1. **上传语音**  
   
   - 路径: `POST /uploadAudio`  
   - 方法签名: [`BoardController.uploadAudio`](app_backend/src/main/java/com/skelig/demo/controller/BoardController.java)  
   - 参数:  
     - form-data 中的 `file` (MultipartFile)  
   - 功能:  
     - 处理并存储用户上传的音频文件，关联 sessionId。  
     - 生成或更新音频得分 (ScoreValue)。  
   - 返回:  
     - JSON 格式的结果 (Result<?>)

2. **上传图片**  
   
   - 路径: `POST /uploadImage`  
   - 方法签名: [`BoardController.uploadImage`](app_backend/src/main/java/com/skelig/demo/controller/BoardController.java)  
   - 参数:  
     - 请求头 `Session-ID`  
     - Body 中的 `byte[]` 图片数据  
   - 功能:  
     - 为当前训练会话上传并存储图片，为后续分析使用。  
   - 返回:  
     - JSON 格式的结果 (Result<?>)

3. **上传心率血氧**  
   
   - 路径: `POST /uploadHeartrateOxygen`  
   - 方法签名: [`BoardController.uploadOxygen`](app_backend/src/main/java/com/skelig/demo/controller/BoardController.java)  
   - 参数:  
     - JSON 对象 HoData  
   - 功能:  
     - 存储用户的心率与血氧数据。  
   - 返回:  
     - JSON 格式的结果 (Result)

4. **开始训练请求**  
   
   - 路径: `GET /upload/getPracBegin`  
   - 方法签名: [`BoardController.getPracBegin`](app_backend/src/main/java/com/skelig/demo/controller/BoardController.java)  
   - 功能:  
     - 生成新的 sessionId 并关联到最近任务。  
   - 返回:  
     - JSON 格式的结果 (Result<?>)

5. **获取训练文本 (已弃用)**  
   
   - 路径: `GET /training/text/{rank}`  
   - 方法签名: [`BoardController.getTrainingText`](app_backend/src/main/java/com/skelig/demo/controller/BoardController.java)  
   - 说明: 提供基础训练文本

6. **获取演讲文本**  
   
   - 路径: `GET /speech/text/{rank}`  
   - 方法签名: `BoardController.getSpeechText(int rank)`  
   - 功能:  
     - 用于获取指定编号的演讲文本  
   - 返回:  
     - JSON 格式的结果 (Result<?>)

7. **一次性综合处理 (已弃用)**  
   
   - 路径: `POST /exerciseover`  
   - 方法签名: `BoardController.getExerciseOver(MultipartFile audiofile, String sessionId)`  
   - 功能:  
     - (已弃用) 综合处理音频并生成建议  
   - 返回:  
     - JSON 格式的结果或音频流

8. **获取当前文本**  
   
   - 路径: `GET /getTheText`  
   - 方法签名: `BoardController.getTheText()`  
   - 功能:  
     - 获取当前会话所使用的文本内容  
   - 返回:  
     - JSON 格式的结果 (Map<String, String>)

9. **获取当前得分**  
   
   - 路径: `GET /getNowScore`  
   - 方法签名: `BoardController.getNowScore()`  
   - 功能:  
     - 返回当前随机生成的得分  
   - 返回:  
     - JSON 格式的结果 (Result<?>)

---

### UserController

位于 [UserController.java](app_backend/src/main/java/com/skelig/demo/controller/UserController.java)，主要负责用户端的查询、统计、任务管理等功能。

1. **查看所有任务记录**  
   
   - 路径: `GET /user/getTasks`  
   - 方法签名: [`UserController.getTasks`](app_backend/src/main/java/com/skelig/demo/controller/UserController.java)  
   - 功能:  
     - 返回数据库中已创建的所有任务。  
   - 返回:  
     - JSON 格式的结果 (Result<?>)

2. **根据 TaskID 获取所有训练记录**  
   
   - 路径: `GET /user/getSessionMsg/{task_id}`  
   - 方法签名: [`UserController.getSessionMsg`](app_backend/src/main/java/com/skelig/demo/controller/UserController.java)  
   - 功能:  
     - 获取对应任务下的所有 session 及其上传的图片、心率血氧等数据。  
   - 返回:  
     - JSON 格式的结果 (Result<?>)

3. **查看口吃统计信息**  
   
   - 路径: `GET /user/Typestat/{task_id}`  
   - 方法签名: [`UserController.getTypeStat`](app_backend/src/main/java/com/skelig/demo/controller/UserController.java)  
   - 功能:  
     - 对指定任务的所有训练口吃数据进行聚合统计 (中断、重复等)。  
   - 返回:  
     - JSON 格式的结果 (Result<?>)

4. **获取平均心率与血氧**  
   
   - 路径: `GET /user/getHodataBytaskid/{task_id}`  
   - 方法签名: [`UserController.getHodataBytaskid`](app_backend/src/main/java/com/skelig/demo/controller/UserController.java)  
   - 功能:  
     - 统计指定任务下所有训练的平均心率和血氧。  
   - 返回:  
     - JSON 格式的结果 (Result<?>)

5. **创建新任务**  
   
   - 路径: `POST /user/buildNewTask/{rank}`  
   - 方法签名: [`UserController.buildNewTask`](app_backend/src/main/java/com/skelig/demo/controller/UserController.java)  
   - 功能:  
     - 根据 rank 参数创建新的任务并写入数据库 (0 为自定义训练，非 0 为基础训练)。  
   - 返回:  
     - JSON 格式的结果 (Result<?>)

---

### ChatController

位于 [ChatController.java](app_backend/src/main/java/com/skelig/demo/controller/ChatController.java)，提供聊天和指导建议相关接口。

1. **实时聊天接口**  
   
   - 路径: `POST /chat`
   - 方法签名: `ChatController.chat(@RequestBody ChatreqData chatreqData)`
   - 参数:
     - JSON 对象 ChatreqData（包括流畅次数、中断次数、重复情况等）
   - 功能:
     - 根据用户提供的统计数据生成特定问题传给语言模型，获取相应回答
   - 返回:
     - JSON 格式的结果 (ChatResponse)

2. **获取随机指导建议**  
   
   - 路径: `GET /getNowAdvice`
   - 方法签名: `ChatController.getNowAdvice()`
   - 参数:
     - 无
   - 功能:
     - 随机生成特定场景下的发音和心理调节建议
   - 返回:
     - JSON 格式的结果 (ChatResponse)

### 其他接口

- `GET /user/getBasicText`  
  获取所有基础训练文本: [`UserController.getBasicText`](app_backend/src/main/java/com/skelig/demo/controller/UserController.java)  
- `GET /user/SessionLogs/{task_id}`  
  获取指定任务下的 session 日志信息: [`UserController.getSessionLogs`](app_backend/src/main/java/com/skelig/demo/controller/UserController.java)
