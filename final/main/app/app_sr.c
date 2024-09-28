/*
 * SPDX-FileCopyrightText: 2015-2023 Espressif Systems (Shanghai) CO LTD
*
* SPDX-License-Identifier: Unlicense OR CC0-1.0
*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "esp_check.h"
#include "esp_err.h"
#include "esp_log.h"
#include "app_sr.h"
#include "esp_afe_sr_models.h"
#include "esp_mn_models.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "app_play_music.h"
#include "app_sr_handler.h"
#include "model_path.h"
#include "esp_mn_speech_commands.h"
#include "esp_process_sdkconfig.h"
#include "bsp_board_extra.h"

#define I2S_CHANNEL_NUM     (2)

static const char *TAG = "app_sr";

static model_iface_data_t       *model_data     = NULL;
static const esp_mn_iface_t     *multinet       = NULL;
static const esp_afe_sr_iface_t *afe_handle     = NULL;
static QueueHandle_t            g_result_que    = NULL;
static srmodel_list_t           *models         = NULL;

const char *cmd_phoneme[12] = {
    "da kai kong qi jing hua qi",
    "guan bi kong qi jing hua qi",
    "da kai tai deng",
    "guan bi tai deng",
    "tai deng tiao liang",
    "tai deng tiao an",
    "da kai deng dai",
    "guan bi deng dai",
    "bo fang yin yue",
    "ting zhi bo fang",
    "da kai shi jian",
    "da kai ri li"
};

// 向缓冲区喂录音数据
static void audio_feed_task(void *pvParam)
{
    size_t bytes_read = 0;
    // 转换传入的参数为音频前端数据结构指针
    esp_afe_sr_data_t *afe_data = (esp_afe_sr_data_t *) pvParam;

    // 从音频前端处理器获取每次应该读取的音频数据块的大小
    int audio_chunksize = afe_handle->get_feed_chunksize(afe_data);
    ESP_LOGI(TAG, "audio_chunksize=%d, feed_channel=%d", audio_chunksize, 3);

    // 为音频数据分配缓冲区，尺寸基于音频块的大小和通道数（这里是3通道）
    int16_t *audio_buffer = heap_caps_malloc(audio_chunksize * sizeof(int16_t) * 3, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (NULL == audio_buffer) {
        esp_system_abort("No mem for audio buffer");  // 如果内存分配失败，则中止程序
    }

    // 循环不结束，不断进行音频数据的读取
    while (true) {
        // 从I2S总线读取音频数据到缓冲区，如果读取失败，记录错误信息
        esp_err_t read_result = bsp_extra_i2s_read((char *)audio_buffer, audio_chunksize * I2S_CHANNEL_NUM * sizeof(int16_t), &bytes_read, portMAX_DELAY);
        if (read_result != ESP_OK) {
            ESP_LOGE(TAG, "======== bsp_extra_i2s_read failed ==========");
        }

        // 调整通道数据，将双通道数据扩展到三通道
        for (int i = audio_chunksize - 1; i >= 0; i--) {
            audio_buffer[i * 3 + 2] = 0;  // 第三通道置零
            audio_buffer[i * 3 + 1] = audio_buffer[i * 2 + 1];  // 复制第二通道
            audio_buffer[i * 3 + 0] = audio_buffer[i * 2 + 0];  // 复制第一通道
        }

        // 将处理后的音频数据送入音频前端引擎
        afe_handle->feed(afe_data, audio_buffer);
    }

    // 如果音频流结束，清理音频前端处理器资源
    afe_handle->destroy(afe_data);

    // 任务结束，删除自身
    vTaskDelete(NULL);
}

// 识别
static void audio_detect_task(void *pvParam)
{
    // 初始化变量，用于标记是否检测到唤醒词
    bool detect_flag = false;
    // 从传入的参数中获取音频前端处理器的数据结构指针
    esp_afe_sr_data_t *afe_data = (esp_afe_sr_data_t *) pvParam;

    // 从音频前端处理器获取音频数据块的大小，并确认与多网模型的样本块大小相匹配
    int afe_chunksize = afe_handle->get_fetch_chunksize(afe_data);
    int mu_chunksize = multinet->get_samp_chunksize(model_data);
    assert(mu_chunksize == afe_chunksize);
    // 开始检测日志
    ESP_LOGI(TAG, "------------detect start------------\n");

    while (true) {
        // 从音频前端获取数据
        afe_fetch_result_t *res = afe_handle->fetch(afe_data);
        if (!res || res->ret_value == ESP_FAIL) {
            // 如果获取数据失败，记录错误并继续
            ESP_LOGE(TAG, "fetch error!");
            continue;
        }

        // 检测到唤醒词
        if (res->wakeup_state == WAKENET_DETECTED) {
            // 记录唤醒词被检测到的日志
            ESP_LOGI(TAG, LOG_BOLD(LOG_COLOR_GREEN) "Wakeword detected");
            sr_result_t result = {
                .wakenet_mode = WAKENET_DETECTED,
                .state = ESP_MN_STATE_DETECTING,
                .command_id = 0,
            };
            // 将结果发送到结果队列
            xQueueSend(g_result_que, &result, 10);
        } else if (res->wakeup_state == WAKENET_CHANNEL_VERIFIED) {
            // 频道验证成功，准备接受指令
            ESP_LOGI(TAG, LOG_BOLD(LOG_COLOR_GREEN) "Channel verified");
            detect_flag = true;
            // 禁用唤醒网络，进入命令检测模式
            afe_handle->disable_wakenet(afe_data);
        }

        // 如果检测到唤醒词或频道验证后
        if (true == detect_flag) {
            // 默认状态为检测中
            esp_mn_state_t mn_state = ESP_MN_STATE_DETECTING;

            // 执行命令检测
            mn_state = multinet->detect(model_data, res->data);

            // 根据检测状态处理
            if (ESP_MN_STATE_DETECTING == mn_state) {
                continue;
            }

            if (ESP_MN_STATE_TIMEOUT == mn_state) {
                // 检测超时处理
                ESP_LOGW(TAG, "Time out");
                sr_result_t result = {
                    .wakenet_mode = WAKENET_NO_DETECT,
                    .state = mn_state,
                    .command_id = 0,
                };
                xQueueSend(g_result_que, &result, 10);
                // 重新启用唤醒网络
                afe_handle->enable_wakenet(afe_data);
                detect_flag = false;
                continue;
            }

            if (ESP_MN_STATE_DETECTED == mn_state) {
                // 检测到命令，获取结果
                esp_mn_results_t *mn_result = multinet->get_results(model_data);
                for (int i = 0; i < mn_result->num; i++) {
                    ESP_LOGI(TAG, "TOP %d, command_id: %d, phrase_id: %d, prob: %f",
                            i + 1, mn_result->command_id[i], mn_result->phrase_id[i], mn_result->prob[i]);
                }

                // 记录检测到的命令
                int sr_command_id = mn_result->command_id[0];
                ESP_LOGI(TAG, "Deteted command : %d", sr_command_id);
                sr_result_t result = {
                    .wakenet_mode = WAKENET_NO_DETECT,
                    .state = mn_state,
                    .command_id = sr_command_id,
                };
                xQueueSend(g_result_que, &result, 10);
#if !SR_CONTINUE_DET
                // 根据设置决定是否继续检测
                afe_handle->enable_wakenet(afe_data);
                detect_flag = false;
#endif
                continue;
            }
            // 处理未捕获的异常
            ESP_LOGE(TAG, "Exception unhandled");
        }
    }

    // 清理资源
    afe_handle->destroy(afe_data);

    // 删除任务
    vTaskDelete(NULL);
}

// 
esp_err_t app_sr_start(void)
{
    // 创建一个结果队列，用于存储音频处理结果
    g_result_que = xQueueCreate(1, sizeof(sr_result_t));
    ESP_RETURN_ON_FALSE(NULL != g_result_que, ESP_ERR_NO_MEM, TAG, "Failed create result queue");

    // 初始化语音模型
    models = esp_srmodel_init("model");

    // 获取AFE处理句柄和配置
    afe_handle = &ESP_AFE_SR_HANDLE;
    afe_config_t afe_config = AFE_CONFIG_DEFAULT();

    // 设置唤醒词模型名称
    afe_config.wakenet_model_name = esp_srmodel_filter(models, ESP_WN_PREFIX, NULL);
    afe_config.aec_init = false;

    // 创建音频前端数据结构
    esp_afe_sr_data_t *afe_data = afe_handle->create_from_config(&afe_config);
    ESP_LOGI(TAG, "load wakenet:%s", afe_config.wakenet_model_name);

    // 获取多网模型名称并创建处理实例
    char *mn_name = esp_srmodel_filter(models, ESP_MN_CHINESE, NULL);
    if (NULL == mn_name) {
        ESP_LOGE(TAG, "No multinet model found");
        return ESP_FAIL;
    }
    multinet = esp_mn_handle_from_name(mn_name);
    model_data = multinet->create(mn_name, 5760);
    ESP_LOGI(TAG, "load multinet:%s", mn_name);

    // 清空并更新命令
    esp_mn_commands_clear();
    for (int i = 0; i < sizeof(cmd_phoneme) / sizeof(cmd_phoneme[0]); i++) {
        esp_mn_commands_add(i, (char *)cmd_phoneme[i]);
    }
    esp_mn_commands_update();
    esp_mn_commands_print();
    multinet->print_active_speech_commands(model_data);

    // 创建音频处理任务
    BaseType_t ret_val = xTaskCreatePinnedToCore(audio_feed_task, "Feed Task", 4 * 1024, afe_data, 5, NULL, 1);
    ESP_RETURN_ON_FALSE(pdPASS == ret_val, ESP_FAIL, TAG, "Failed create audio feed task");

    ret_val = xTaskCreatePinnedToCore(audio_detect_task, "Detect Task", 6 * 1024, afe_data, 5, NULL, 0);
    ESP_RETURN_ON_FALSE(pdPASS == ret_val, ESP_FAIL, TAG, "Failed create audio detect task");

    ret_val = xTaskCreatePinnedToCore(sr_handler_task, "SR Handler Task", 4 * 1024, g_result_que, 1, NULL, 1);
    ESP_RETURN_ON_FALSE(pdPASS == ret_val, ESP_FAIL, TAG, "Failed create audio handler task");

    return ESP_OK;
}


esp_err_t app_sr_reset_command_list(char *command_list)
{
    char *err_id = heap_caps_malloc(1024, MALLOC_CAP_SPIRAM);   //分配了 1024 字节的内存，并将内存指针存储在 err_id 变量中。此内存分配尝试使用 SPI RAM（如果可用）。
    ESP_RETURN_ON_FALSE(NULL != err_id, ESP_ERR_NO_MEM, TAG,  "memory is not enough");      //通过 ESP_RETURN_ON_FALSE 宏检查 err_id 是否为 NULL，即检查内存是否成功分配。如果内存分配失败（err_id 为 NULL），函数将返回 ESP_ERR_NO_MEM 错误码，并记录错误消息 "memory is not enough"。
    free(err_id);       //随后立即释放之前分配的内存，这意味着在这个函数中内存分配并未实际用于持久存储数据，可能仅用于检验内存分配是否成功。
    return ESP_OK;
}