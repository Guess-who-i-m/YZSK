#include <stdio.h>
#include <stdlib.h>//
#include <string.h>//
#include "esp_err.h"//
#include "esp_log.h"//
#include "esp_wifi.h"//
#include "esp_system.h"//
#include "nvs_flash.h"//
#include "esp_event.h"//
#include "esp_netif.h"//
#include "esp_tls.h"//
#include "esp_http_client.h"

#include "protocol_examples_common.h"//
#include "protocol_examples_utils.h"
#include "freertos/FreeRTOS.h"//
#include "freertos/task.h"//
#include "freertos/event_groups.h"//
#include "bsp/esp-box-3.h"
#include "bsp_board_extra.h"

// #include "app_play_music.h"
#include "app_sr.h"
#include "app_max30102.h"
#include "wifi.h"
#include "audio.h"
#include "app_uvc_camera.h"
#include "lvgl.h"
#include "ui/ui.h"
#define APP_DISP_DEFAULT_BRIGHTNESS 50

static const char *TAG = "main";

EventGroupHandle_t task_event_group;
#define TASK_START_BIT (1 << 0)
// 定义任务句柄
static TaskHandle_t max30102_task_handle = NULL;
static TaskHandle_t camera_task_handle = NULL;
static TaskHandle_t audio_task_handle = NULL;
void nvs_init( )
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}
// *INDENT-OFF*
void app_lvgl_display(void)
{
    bsp_display_lock(0);

    ui_init();

    bsp_display_unlock();
}

void app_main(void)
{
    // 初始化NVS
    nvs_init();

    // 初始化WiFi连接
    ESP_LOGI("WIFI:", "ESP_WIFI_MODE_STA!");
    wifi_init_sta();

    // 初始化I2C和其他硬件资源
    bsp_i2c_init();

    // 初始化显示和LVGL
    bsp_display_start();
    bsp_display_brightness_set(APP_DISP_DEFAULT_BRIGHTNESS);
    app_lvgl_display();

    // 初始化音频资源
    bsp_extra_codec_init();

    // 初始化事件组
    task_event_group = xEventGroupCreate();
    if (task_event_group == NULL) {
        ESP_LOGE(TAG, "Failed to create event group");
        return;
    }

    // 创建任务
    xTaskCreatePinnedToCore(audio_feed_task, "Audio Feed Task", 8 * 1024, NULL, 5, &audio_task_handle, 1);
    xTaskCreate(camera_task, "Camera Task", 16 * 1024, NULL, 5, &camera_task_handle);
    xTaskCreate(max30102_update_task, "max30102_update_task", 8 * 1024, NULL, 5, &max30102_task_handle);

    // 打印初始内存使用情况
    ESP_LOGI(TAG, "Free memory after start: %d bytes", heap_caps_get_total_size(MALLOC_CAP_INTERNAL));

    // 主循环
    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
