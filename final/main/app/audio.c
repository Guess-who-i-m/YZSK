#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"
#include "bsp_board_extra.h"
#include "audio.h"
#include "esp_http_client.h"
#include "task_flags.h"
#include "freertos/event_groups.h"

#define I2S_CHANNEL_NUM     (2)
#define AUDIO_THRESHOLD     1000  // 声音检测阈值，根据需要调整
#define AUDIO_CHUNKSIZE     4096  // 较小的音频块大小
#define PSRAM_BUFFER_SIZE   (1024 * 1024)  // 1MB PSRAM缓存大小
#define SERVER_URL "http://arnc-2024.natapp1.cc/uploadAudio"
#define SAMPLE_RATE         16000
#define BITS_PER_SAMPLE     16
#define NUM_CHANNELS        1
static const char *TAG = "audio_feed";
int16_t *psram_audio_buffer = NULL;
size_t psram_buffer_pos = 0;
extern EventGroupHandle_t task_event_group;

// WAV文件头格式
typedef struct {
    uint8_t  chunk_id[4];       // "RIFF"
    uint32_t chunk_size;        // 文件大小减去8字节
    uint8_t  format[4];         // "WAVE"
    uint8_t  subchunk1_id[4];   // "fmt "
    uint32_t subchunk1_size;    // 16 for PCM
    uint16_t audio_format;      // PCM = 1
    uint16_t num_channels;      // 1 for mono, 2 for stereo
    uint32_t sample_rate;       // 采样率，如44100
    uint32_t byte_rate;         // == sample_rate * num_channels * bits_per_sample/8
    uint16_t block_align;       // == num_channels * bits_per_sample/8
    uint16_t bits_per_sample;   // 8 bits = 8, 16 bits = 16
    uint8_t  subchunk2_id[4];   // "data"
    uint32_t subchunk2_size;    // 数据大小
} wav_header_t;

void generate_wav_header(wav_header_t *header, uint32_t num_samples, uint16_t num_channels, uint32_t sample_rate, uint16_t bits_per_sample) {
    memcpy(header->chunk_id, "RIFF", 4);
    header->chunk_size = 36 + num_samples * num_channels * bits_per_sample / 8;
    memcpy(header->format, "WAVE", 4);
    memcpy(header->subchunk1_id, "fmt ", 4);
    header->subchunk1_size = 16;
    header->audio_format = 1;
    header->num_channels = num_channels;
    header->sample_rate = sample_rate;
    header->byte_rate = sample_rate * num_channels * bits_per_sample / 8;
    header->block_align = num_channels * bits_per_sample / 8;
    header->bits_per_sample = bits_per_sample;
    memcpy(header->subchunk2_id, "data", 4);
    header->subchunk2_size = num_samples * num_channels * bits_per_sample / 8;
}
static esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            break;
        case HTTP_EVENT_REDIRECT:
            ESP_LOGI(TAG, "HTTP_EVENT_REDIRECT");
            break;
    }
    return ESP_OK;
}

static void upload_audio_to_server(const char* session_id, const int16_t* audio_data, size_t num_samples) {
    const char* boundary = "----ESP32Boundary";
    const char* content_type = "multipart/form-data; boundary=----ESP32Boundary";

    // 构建多部分表单数据的开始部分，包含 session_id
    char form_data_start[512];
    snprintf(form_data_start, sizeof(form_data_start),
        "--%s\r\n"
        "Content-Disposition: form-data; name=\"session_id\"\r\n\r\n%s\r\n"
        "--%s\r\n"
        "Content-Disposition: form-data; name=\"file\"; filename=\"audio.wav\"\r\n"
        "Content-Type: audio/wav\r\n\r\n",
        boundary, session_id, boundary);

    // 构建多部分表单数据的结束部分
    char form_data_end[64];
    snprintf(form_data_end, sizeof(form_data_end), "\r\n--%s--\r\n", boundary);

    // 生成WAV文件头
    wav_header_t wav_header;
    generate_wav_header(&wav_header, num_samples, 2, 16000, 16);  // 假设采样率为44100Hz，16位样本，立体声

    // 计算整个请求体的长度
    size_t body_length = sizeof(wav_header) + num_samples * sizeof(int16_t) + strlen(form_data_start) + strlen(form_data_end);

    esp_http_client_config_t config = {
        .url = SERVER_URL,
        .event_handler = _http_event_handler,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_header(client, "Content-Type", content_type);

    char content_length[32];
    snprintf(content_length, sizeof(content_length), "%zu", body_length);
    esp_http_client_set_header(client, "Content-Length", content_length);

    esp_http_client_open(client, body_length);

    esp_http_client_write(client, form_data_start, strlen(form_data_start));
    esp_http_client_write(client, (const char*)&wav_header, sizeof(wav_header));
    esp_http_client_write(client, (const char*)audio_data, num_samples * sizeof(int16_t));
    esp_http_client_write(client, form_data_end, strlen(form_data_end));

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %lld",
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}

void audio_feed_task(void *pvParam) {
    int16_t *audio_buffer = NULL;
    size_t bytes_read = 0;
    ESP_LOGI(TAG, "Audio recording task started.");

    // 一次性分配内存
    audio_buffer = heap_caps_malloc(AUDIO_CHUNKSIZE * sizeof(int16_t) * 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (audio_buffer == NULL) {
        ESP_LOGE(TAG, "No memory for audio buffer");
        vTaskDelete(NULL);
        return;
    }

    psram_audio_buffer = heap_caps_malloc(PSRAM_BUFFER_SIZE * sizeof(int16_t), MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (psram_audio_buffer == NULL) {
        ESP_LOGE(TAG, "No memory for PSRAM audio buffer");
        heap_caps_free(audio_buffer);
        vTaskDelete(NULL);
        return;
    }

    while (1) {
        // 等待任务启动信号
        xEventGroupWaitBits(task_event_group, TASK_START_BIT, pdFALSE, pdTRUE, portMAX_DELAY);

        // 读取音频数据
        esp_err_t read_result = bsp_extra_i2s_read((char *)audio_buffer, AUDIO_CHUNKSIZE * I2S_CHANNEL_NUM * sizeof(int16_t), &bytes_read, portMAX_DELAY);
        if (read_result != ESP_OK) {
            ESP_LOGE(TAG, "======== bsp_extra_i2s_read failed ==========");
            vTaskDelay(pdMS_TO_TICKS(100)); // 避免连续错误，增加延迟
            continue;  // 如果读取失败，继续下一次读取
        }

        if (bytes_read > 0) {
            ESP_LOGI(TAG, "Read %d bytes from I2S", bytes_read);

            // 检查是否有足够的空间存储新的音频数据
            if (psram_buffer_pos + bytes_read <= PSRAM_BUFFER_SIZE) {
                memcpy(psram_audio_buffer + psram_buffer_pos / sizeof(int16_t), audio_buffer, bytes_read);
                psram_buffer_pos += bytes_read;
            } else {
                ESP_LOGW(TAG, "PSRAM buffer full, uploading audio data");
                const char *session_id = "sess_1721311316684";  // 替换为实际的session_id
                upload_audio_to_server(session_id, psram_audio_buffer, psram_buffer_pos / sizeof(int16_t));
                psram_buffer_pos = 0;
            }
        }

        // 检查是否检测到声音
        bool sound_detected = false;
        for (int i = 0; i < AUDIO_CHUNKSIZE; i++) {
            if (abs(audio_buffer[i * 2]) > AUDIO_THRESHOLD || abs(audio_buffer[i * 2 + 1]) > AUDIO_THRESHOLD) {
                sound_detected = true;
                break;
            }
        }

        if (sound_detected) {
            ESP_LOGI(TAG, "Sound detected, audio buffer filling up...");
        }

        // 等待任务暂停信号
        if ((xEventGroupGetBits(task_event_group) & TASK_START_BIT) == 0) {
            ESP_LOGI(TAG, "Task paused, waiting to resume.");
            xEventGroupWaitBits(task_event_group, TASK_START_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
            ESP_LOGI(TAG, "Task resumed.");
        }
    }

    // 释放缓冲区内存
    heap_caps_free(audio_buffer);
    heap_caps_free(psram_audio_buffer);
    vTaskDelete(NULL);
}

// void start_audio_feed_task(void)
// {
//     BaseType_t ret_val = xTaskCreatePinnedToCore(audio_feed_task, "Feed Task", 8 * 1024, NULL, 5, NULL, 1);

// }
