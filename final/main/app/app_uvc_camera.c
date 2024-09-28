#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_heap_caps.h"
#include "usb_stream.h"
#include "esp_http_client.h"
#include "esp_tls.h"
#include "app_uvc_camera.h"
#include "cJSON.h"
#include "task_flags.h"

static const char *TAG = "app_uvc_camera";

#define BIT0_FRAME_START            (0x01 << 0)
#define BIT1_NEW_FRAME_START        (0x01 << 1)
#define BIT2_NEW_FRAME_END          (0x01 << 2)
#define BIT3_SPK_START              (0x01 << 3)
#define BIT4_SPK_RESET              (0x01 << 4)

static uint8_t *frame_buffer = NULL;
static TimerHandle_t timer = NULL;
extern EventGroupHandle_t task_event_group;

// 摄像头帧回调函数，将捕获的图像数据拷贝到frame_buffer中
static void camera_frame_cb(uvc_frame_t *frame, void *ptr) {
    memcpy(frame_buffer, frame->data, frame->data_bytes);
    ESP_LOGI(TAG, "Captured frame size: %u", frame->data_bytes);
}

// HTTP事件处理函数，用于处理HTTP请求的各种事件
static esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
    static char *output_buffer;
    static int output_len;
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            if (!esp_http_client_is_chunked_response(evt->client)) {
                if (evt->user_data) {
                    memcpy(evt->user_data + output_len, evt->data, evt->data_len);
                } else {
                    if (output_buffer == NULL) {
                        output_buffer = (char *) malloc(esp_http_client_get_content_length(evt->client));
                        output_len = 0;
                        if (output_buffer == NULL) {
                            ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                            return ESP_FAIL;
                        }
                    }
                    memcpy(output_buffer + output_len, evt->data, evt->data_len);
                }
                output_len += evt->data_len;
            }
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
            if (output_buffer != NULL) {
                ESP_LOG_BUFFER_HEX(TAG, output_buffer, output_len);
                free(output_buffer);
                output_buffer = NULL;
            }
            output_len = 0;
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            int mbedtls_err = 0;
            esp_err_t err = esp_tls_get_and_clear_last_error((esp_tls_error_handle_t)evt->data, &mbedtls_err, NULL);
            if (err != 0) {
                ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
                ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
            }
            if (output_buffer != NULL) {
                free(output_buffer);
                output_buffer = NULL;
            }
            output_len = 0;
            break;
        case HTTP_EVENT_REDIRECT:
            ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
            esp_http_client_set_header(evt->client, "From", "user@example.com");
            esp_http_client_set_header(evt->client, "Accept", "text/html");
            break;
    }
    return ESP_OK;
}

// 发送图像数据的HTTP POST请求，添加session_id
static esp_err_t http_post_image_data(const char *session_id, const uint8_t *image_data, size_t length) {
    esp_http_client_config_t config = {
        .url = "http://arnc-2024.natapp1.cc/uploadImage",
        .event_handler = _http_event_handler,
        .disable_auto_redirect = true,
        .timeout_ms = 10000,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_header(client, "Content-Type", "application/octet-stream");
    esp_http_client_set_header(client, "Session-ID", session_id);
    esp_http_client_set_post_field(client, (const char *)image_data, length);

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %"PRId64,
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
    return err;
}

// 捕获并发送图像数据的定时器回调函数
void capture_and_send_image(TimerHandle_t xTimer) {
    if (frame_buffer != NULL) {
        const char *session_id = "your_session_id_here";  // 替换为实际的session_id
        ESP_LOGI(TAG, "Sending captured image to backend");
        http_post_image_data(session_id, frame_buffer, DEMO_UVC_XFER_BUFFER_SIZE);
    }
}

// 初始化摄像头并启动定时器
void app_uvc_camera_init(void) {
    uint8_t *xfer_buffer_a = (uint8_t *)heap_caps_malloc(DEMO_UVC_XFER_BUFFER_SIZE, MALLOC_CAP_SPIRAM);
    assert(xfer_buffer_a != NULL);
    uint8_t *xfer_buffer_b = (uint8_t *)heap_caps_malloc(DEMO_UVC_XFER_BUFFER_SIZE, MALLOC_CAP_SPIRAM);
    assert(xfer_buffer_b != NULL);

    frame_buffer = (uint8_t *)heap_caps_malloc(DEMO_UVC_XFER_BUFFER_SIZE, MALLOC_CAP_SPIRAM);
    assert(frame_buffer != NULL);

    uvc_config_t uvc_config = {
        .frame_width        = DEMO_UVC_FRAME_WIDTH,
        .frame_height       = DEMO_UVC_FRAME_HEIGHT,
        .frame_interval     = FPS2INTERVAL(15),
        .xfer_buffer_size   = DEMO_UVC_XFER_BUFFER_SIZE,
        .xfer_buffer_a      = xfer_buffer_a,
        .xfer_buffer_b      = xfer_buffer_b,
        .frame_buffer_size  = DEMO_UVC_XFER_BUFFER_SIZE,
        .frame_buffer       = frame_buffer,
        .frame_cb           = &camera_frame_cb,
        .frame_cb_arg       = NULL,
    };

    esp_err_t ret = uvc_streaming_config(&uvc_config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "uvc streaming config failed: %s", esp_err_to_name(ret));
        return; // 如果配置失败，退出函数
    }

    ret = usb_streaming_start();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "usb streaming start failed: %s", esp_err_to_name(ret));
        return; // 如果启动失败，退出函数
    }

    // 创建定时器，每隔5s调用一次回调函数
    timer = xTimerCreate("CaptureTimer", pdMS_TO_TICKS(5000), pdTRUE, NULL, capture_and_send_image);
    if (timer == NULL) {
        ESP_LOGE(TAG, "Failed to create timer");
    } else {
        xTimerStart(timer, 0);
    }
}

// 摄像头任务
void camera_task(void *pvParameters) {
    while (1) {
        // 等待事件组中的 TASK_START_BIT 被设置，启动任务
        xEventGroupWaitBits(task_event_group, TASK_START_BIT, pdFALSE, pdTRUE, portMAX_DELAY);

        app_uvc_camera_init();
        camera_task_running = true;

        while (1) {
            // 检查 TASK_START_BIT 是否仍然被设置
            EventBits_t bits = xEventGroupGetBits(task_event_group);
            if (!(bits & TASK_START_BIT)) {
                // 如果 TASK_START_BIT 被清除，暂停任务
                ESP_LOGI(TAG, "Camera task is paused.");
                break; // 退出内部循环，任务将被暂停
            }

            // 任务的主要工作
            vTaskDelay(pdMS_TO_TICKS(1000)); // 每秒检查一次
        }

        // 停止摄像头流
        usb_streaming_stop();

        // 释放定时器
        if (timer != NULL) {
            xTimerStop(timer, 0);
            xTimerDelete(timer, 0);
            timer = NULL;
        }

        // 释放缓冲区
        if (frame_buffer != NULL) {
            heap_caps_free(frame_buffer);
            frame_buffer = NULL;
        }

        // 任务在这里等待暂停和恢复信号
    }
}