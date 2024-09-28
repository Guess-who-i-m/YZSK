#include <string.h>
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_check.h"
#include "app_sr.h"
#include "app_sr_handler.h"
#include "esp_afe_sr_iface.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "app_play_music.h"
#include "app_max30102.h"
#include "app_uvc_camera.h"

static const char *TAG = "app_sr_handler";



void sr_handler_task(void *pvParam)
{
    // 从参数中获取结果队列的句柄
    QueueHandle_t xQueue = (QueueHandle_t) pvParam;

    while (true) {
        sr_result_t result;
        // 从队列中接收语音识别结果，永久等待直到有结果可读
        xQueueReceive(xQueue, &result, portMAX_DELAY);

        // 记录接收到的命令和状态
        ESP_LOGI(TAG, "cmd:%d, wakemode:%d,state:%d", result.command_id, result.wakenet_mode, result.state);

        // 如果识别结果状态为超时，则处理超时逻辑
        if (ESP_MN_STATE_TIMEOUT == result.state) {
            ESP_LOGI(TAG, "timeout");
            // // 超时时更新UI元素状态
            // lv_obj_clear_flag(ui_cat_gif, LV_OBJ_FLAG_HIDDEN);
            // lv_obj_add_flag(ui_Image1, LV_OBJ_FLAG_HIDDEN);
            // lv_obj_add_flag(ui_Labelwakenet, LV_OBJ_FLAG_HIDDEN);
            continue;
        }

        // 如果检测到唤醒词
        if (WAKENET_DETECTED == result.wakenet_mode) {
            ESP_LOGI(TAG, "wakenet detected");

            continue;
        }

        // 如果语音命令已识别
        if (ESP_MN_STATE_DETECTED & result.state) {
            ESP_LOGI(TAG, "mn detected");

            // 根据识别到的命令ID执行相应的操作
            switch (result.command_id) {
                // 不同命令ID对应不同设备控制或UI更新
                case 0:
                    // 例如，打开空气净化器
                    // mqtt_air_purifer_on();
                    // lv_label_set_text(ui_Labelwakenet, "已打开");
                    ESP_LOGI(TAG, "打开空气净化器");
                    break;
                case 1:
                    // 关闭空气净化器
                    // mqtt_air_purifer_off();
                    // lv_label_set_text(ui_Labelwakenet, "已关闭");
                    ESP_LOGI(TAG, "关闭空气净化器");
                    break;
                case 2:
                    ESP_LOGI(TAG, "打开台灯");
                    // mqtt_lamp_on();
                    // lv_label_set_text(ui_Labelwakenet, "已打开");
                    break;
                case 3:
                    ESP_LOGI(TAG, "关闭台灯");
                    // mqtt_lampr_off();
                    // lv_label_set_text(ui_Labelwakenet, "已关闭");
                    break;
                case 4:
                    ESP_LOGI(TAG, "调高亮度");
                    // mqtt_lamp_brighter();
                    // lv_label_set_text(ui_Labelwakenet, "好的！");
                    break;
                case 5:
                    ESP_LOGI(TAG, "调低亮度");
                    // mqtt_lampr_dimmer();
                    // lv_label_set_text(ui_Labelwakenet, "好的！");
                    break;
                case 6:
                    ESP_LOGI(TAG, "打开灯带");
                    // mqtt_led_on();
                    // lv_label_set_text(ui_Labelwakenet, "已打开");


                    break;
                case 7:
                    ESP_LOGI(TAG, "关闭灯带");
                    // mqtt_led_off();
                    // lv_label_set_text(ui_Labelwakenet, "已关闭");
                    break;
                case 8:
                    ESP_LOGI(TAG, "播放音乐");
                    // lv_label_set_text(ui_Labelwakenet, "好的！");
                    // _ui_screen_change(&ui_Screen4, LV_SCR_LOAD_ANIM_FADE_ON, 100, 0, &ui_Screen4_screen_init);
                    music_play_resume();
                    break;
                case 9:
                    ESP_LOGI(TAG, "暂停音乐");
                    // lv_label_set_text(ui_Labelwakenet, "好的！");
                    // _ui_screen_change(&ui_Screen1, LV_SCR_LOAD_ANIM_FADE_ON, 100, 0, &ui_Screen4_screen_init);
                    music_play_pause();
                    break;
                case 10:
                    ESP_LOGI(TAG, "展示时间");
                    // lv_label_set_text(ui_Labelwakenet, "好的！");
                    // _ui_screen_change(&ui_Screen1, LV_SCR_LOAD_ANIM_FADE_ON, 100, 0, &ui_Screen1_screen_init);
                    break;
                case 11:
                    ESP_LOGI(TAG, "展示日历");
                    // lv_label_set_text(ui_Labelwakenet, "好的！");
                    // _ui_screen_change(&ui_Screen3, LV_SCR_LOAD_ANIM_FADE_ON, 100, 0, &ui_Screen1_screen_init);
                    break;
                default:
                    break;
            }
            /* 在此注册更多命令回调 */
        }
    }

    // 任务结束，删除任务
    vTaskDelete(NULL);
}
