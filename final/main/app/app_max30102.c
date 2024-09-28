#include "driver/i2c.h"
#include "bsp/esp-box-3.h"
#include "esp_log.h"
#include "esp_check.h"
#include "max30102.h"
#include "esp_http_client.h"
#include "esp_tls.h"
#include "cJSON.h"
#include <stdbool.h>
#include "freertos/event_groups.h"
#include "task_flags.h"
#include "../ui/ui.h"  // 确保包含了 UI 相关的头文件

extern EventGroupHandle_t task_event_group;

#define I2C_SDA_PIN                     (GPIO_NUM_40)
#define I2C_SCL_PIN                     (GPIO_NUM_41)
#define I2C_NUM                         0
#define I2C_EXPAND_CLK_SPEED_HZ         400000
#define MAX_HTTP_OUTPUT_BUFFER          2048

static const char *TAG = "app_max30102";

static esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    static char *output_buffer;  // Buffer to store response of HTTP request from event handler
    static int output_len;       // Stores number of bytes read
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

// Function to send data to the backend
static esp_err_t http_post_data(const char* session_id, const float *heart_rate_values, const float *spo2_values, size_t length)
{
    char post_data[2048]; // Increase the buffer size if necessary
    int offset = snprintf(post_data, sizeof(post_data), "{\"session_id\":\"%s\",\"heart_rate\":[", session_id);
    for (size_t i = 0; i < length; ++i) {
        offset += snprintf(post_data + offset, sizeof(post_data) - offset, "%f,", heart_rate_values[i]);
    }
    snprintf(post_data + offset - 1, sizeof(post_data) - offset, "],\"spo2\":[");
    offset = strlen(post_data);
    for (size_t i = 0; i < length; ++i) {
        offset += snprintf(post_data + offset, sizeof(post_data) - offset, "%f,", spo2_values[i]);
    }
    snprintf(post_data + offset - 1, sizeof(post_data) - offset, "]}");

    ESP_LOGI(TAG, "JSON data to send: %s", post_data);

    char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};

    esp_http_client_config_t config = {
        .url = "http://arnc-2024.natapp1.cc/uploadHeartrateOxygen",
        .event_handler = _http_event_handler,
        .user_data = local_response_buffer,  // Pass address of local buffer to get response
        .disable_auto_redirect = true,
        .timeout_ms = 10000,  // Increase timeout to 10 seconds
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_method(client, HTTP_METHOD_POST);

    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, post_data, strlen(post_data));

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %"PRId64,
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
    }

    cJSON *root = cJSON_Parse(local_response_buffer);
    if (root) {
        ESP_LOGI(TAG, "HTTP POST Response: %s", local_response_buffer);
        cJSON_Delete(root);
    }

    esp_http_client_cleanup(client);
    return err;
}

static esp_err_t i2c_init()
{
    i2c_config_t conf = {
        .mode               = I2C_MODE_MASTER,
        .sda_io_num         = I2C_SDA_PIN,
        .sda_pullup_en      = GPIO_PULLUP_ENABLE,
        .scl_io_num         = I2C_SCL_PIN,
        .scl_pullup_en      = GPIO_PULLUP_ENABLE,
        .master.clk_speed   = I2C_EXPAND_CLK_SPEED_HZ
    };
    i2c_param_config(I2C_NUM, &conf);
    i2c_driver_install(I2C_NUM, I2C_MODE_MASTER, 0, 0, 0);
    return ESP_OK;
}

void i2c_scan() {
    ESP_LOGI(TAG, "Scanning I2C bus...\n");
    for (uint8_t i = 1; i < 127; i++) {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (i << 1) | I2C_MASTER_WRITE, true);
        i2c_master_stop(cmd);
        esp_err_t ret = i2c_master_cmd_begin(I2C_NUM, cmd, 100 / portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "Device found at address 0x%02X\n", i);
        }
    }
    ESP_LOGI(TAG, "I2C scan completed.");
}

void max30102_update_task(void *pvParam) {
    max30102_task_running = true;
    i2c_init();
    while (max30102_task_running) {
        max30102_handle_t max30102_handle = NULL;
        max30102_data_t data = {0};
        max30102_create(I2C_NUM, &max30102_handle);
        max30102_config(max30102_handle);

        ESP_LOGI(TAG, "Start to read HR and SPO2");
        float heart_rate_values[10] = {0};
        float spo2_values[10] = {0};
        uint32_t data_collected = 0;

        // 等待手检测成功 8 次
        for (int i = 0; i < 4; ) {
            ESP_ERROR_CHECK(max30102_get_data(max30102_handle, &data));
            vTaskDelay(10 / portTICK_PERIOD_MS);
            if (data.hand_detected) {
                i++;
            }
        }

        // 读取 10 次数据
        while (data_collected < 10) {
            ESP_ERROR_CHECK(max30102_get_data(max30102_handle, &data));
            if (data.hand_detected) {
                ESP_LOGI(TAG, "BPM: %f, SaO2: %f", data.heart_rate, data.spo2);
                heart_rate_values[data_collected] = data.heart_rate;
                spo2_values[data_collected] = data.spo2;

                // 渲染当前数据到屏幕
                update_chart_data(&heart_rate_values[data_collected], &spo2_values[data_collected], 1);

                data_collected++;
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        
        const char* session_id = "sess_1721311316684";
        ESP_LOGI(TAG, "Collected data, sending to backend...");
        esp_err_t result = http_post_data(session_id, heart_rate_values, spo2_values, 10);
        if (result == ESP_OK) {
            ESP_LOGI(TAG, "Data sent to backend successfully.");
        } else {
            ESP_LOGE(TAG, "Failed to send data to backend.");
        }

        ESP_LOGI(TAG, "Resetting data collection...");
        memset(heart_rate_values, 0, sizeof(heart_rate_values));
        memset(spo2_values, 0, sizeof(spo2_values));
        data_collected = 0;

        max30102_deinit(max30102_handle);

        // 延迟3秒之后开始下一次记录
        vTaskDelay(pdMS_TO_TICKS(3000)); 
    }
    vTaskDelete(NULL);  // 自删除任务
}
