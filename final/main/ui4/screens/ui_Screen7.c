#include "../ui.h"
#include "esp_http_client.h"
#include "cJSON.h"
#include <string.h>
#include "esp_log.h"  // 添加日志模块
#include "../../main/app/http.h"  // 添加这个头文件

static const char *TAG = "UI_SCREEN7";  // 日志标签

// 声明全局变量
extern lv_obj_t *ui_TextArea2;
extern lv_obj_t *ui_Label15;

// 获取建议
static void fetch_advice() {
    ESP_LOGI(TAG, "Starting to fetch advice...");

    char *local_response_buffer = (char *)heap_caps_malloc(2048, MALLOC_CAP_SPIRAM);

    if (!local_response_buffer) {
        ESP_LOGE(TAG, "Failed to allocate memory for response buffer");
        return;
    }

    esp_http_client_config_t config = {
        .url = "http://arnc-2024.natapp1.cc/getNowAdvice",  // 正确的API路径
        .event_handler = _http_event_handler,
        .user_data = local_response_buffer,  // 使用缓冲区来获取响应
        .timeout_ms = 20000,  // 超时时间 20 秒
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %"PRIu64,
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));
        ESP_LOGD(TAG, "HTTP Response: %s", local_response_buffer);

        // 解析 JSON 并更新 UI
        cJSON *root = cJSON_Parse(local_response_buffer);
        if (root) {
            cJSON *output = cJSON_GetObjectItem(root, "output");
            if (output) {
                const char *advice = cJSON_GetObjectItem(output, "text")->valuestring;
                if (advice) {
                    ESP_LOGI(TAG, "Updating UI with advice: %s", advice);
                    lv_textarea_set_text(ui_TextArea2, advice);  // 更新建议文本框
                    lv_obj_invalidate(ui_TextArea2);  // 手动刷新 UI
                } else {
                    ESP_LOGE(TAG, "Failed to get 'text' from JSON");
                    lv_textarea_set_text(ui_TextArea2, "无法获取建议，请稍后再试。");
                }
            } else {
                ESP_LOGE(TAG, "Failed to find 'output' in JSON");
                lv_textarea_set_text(ui_TextArea2, "无法获取建议，请稍后再试。");
            }
            cJSON_Delete(root);
        } else {
            ESP_LOGE(TAG, "Failed to parse JSON, check response: %s", local_response_buffer);
            lv_textarea_set_text(ui_TextArea2, "数据解析失败，请稍后再试。");
        }
    } else {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
        lv_textarea_set_text(ui_TextArea2, "获取数据失败，请稍后再试。");
    }

    esp_http_client_cleanup(client);
    free(local_response_buffer);
}

static void fetch_score() {
    ESP_LOGI(TAG, "Starting to fetch score...");

    char *local_response_buffer = (char *)heap_caps_malloc(2048, MALLOC_CAP_SPIRAM);

    if (!local_response_buffer) {
        ESP_LOGE(TAG, "Failed to allocate memory for response buffer");
        return;
    }

    esp_http_client_config_t config = {
        .url = "http://arnc-2024.natapp1.cc/getNowScore",  // 正确的API路径
        .event_handler = _http_event_handler,
        .user_data = local_response_buffer,  // 使用缓冲区来获取响应
        .timeout_ms = 10000,  // 超时时间 10 秒
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %"PRIu64,
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));
        ESP_LOGD(TAG, "HTTP Response: %s", local_response_buffer);

        // 解析 JSON 并更新 UI
        cJSON *root = cJSON_Parse(local_response_buffer);
        if (root) {
            cJSON *data = cJSON_GetObjectItem(root, "data");
            if (data) {
                int score = cJSON_GetObjectItem(data, "score")->valueint;
                char score_str[32];
                snprintf(score_str, sizeof(score_str), "本次得分: %d", score);

                // 在 UI 上下文中更新
                ESP_LOGI(TAG, "Updating UI with score: %s", score_str);
                lv_label_set_text(ui_Label15, score_str);  // 更新得分标签
                lv_obj_invalidate(ui_Label15);  // 手动刷新 UI
            } else {
                ESP_LOGE(TAG, "Failed to find 'data' in JSON");
                lv_label_set_text(ui_Label15, "无法获取得分，请稍后再试。");
            }
            cJSON_Delete(root);
        } else {
            ESP_LOGE(TAG, "Failed to parse JSON, check response: %s", local_response_buffer);
            lv_label_set_text(ui_Label15, "数据解析失败，请稍后再试。");
        }
    } else {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
        lv_label_set_text(ui_Label15, "获取数据失败，请稍后再试。");
    }

    esp_http_client_cleanup(client);
    free(local_response_buffer);
}
// 在界面加载完成后获取数据
static void on_screen_load(lv_event_t * e) {
    fetch_advice();
    fetch_score();
}

void ui_Screen7_screen_init(void)
{
    ui_Screen7 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen7, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Panel8 = lv_obj_create(ui_Screen7);
    lv_obj_set_width(ui_Panel8, 314);
    lv_obj_set_height(ui_Panel8, 30);
    lv_obj_set_x(ui_Panel8, 0);
    lv_obj_set_y(ui_Panel8, -103);
    lv_obj_set_align(ui_Panel8, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel8, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Panel8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Panel8, lv_color_hex(0x33B8DA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_Panel8, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Panel8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label17 = lv_label_create(ui_Panel8);
    lv_obj_set_width(ui_Label17, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label17, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label17, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label17, "结果");
    lv_obj_set_style_text_font(ui_Label17, &ui_font_Font1, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button5 = lv_btn_create(ui_Screen7);
    lv_obj_set_width(ui_Button5, 93);
    lv_obj_set_height(ui_Button5, 35);
    lv_obj_set_x(ui_Button5, 102);
    lv_obj_set_y(ui_Button5, 85);
    lv_obj_set_align(ui_Button5, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button5, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button5, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Button5, lv_color_hex(0x33B8D7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label20 = lv_label_create(ui_Button5);
    lv_obj_set_width(ui_Label20, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label20, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label20, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label20, "再次训练");
    lv_obj_set_style_text_font(ui_Label20, &ui_font_Font1, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel2 = lv_obj_create(ui_Screen7);
    lv_obj_set_width(ui_Panel2, 192);
    lv_obj_set_height(ui_Panel2, 34);
    lv_obj_set_x(ui_Panel2, -49);
    lv_obj_set_y(ui_Panel2, 85);
    lv_obj_set_align(ui_Panel2, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Label7 = lv_label_create(ui_Panel2);
    lv_obj_set_width(ui_Label7, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label7, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label7, 0);
    lv_obj_set_y(ui_Label7, 1);
    lv_obj_set_align(ui_Label7, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label7, "使用app以查看详细记录");
    lv_obj_set_style_text_font(ui_Label7, &ui_font_Font1, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel4 = lv_obj_create(ui_Screen7);
    lv_obj_set_width(ui_Panel4, 143);
    lv_obj_set_height(ui_Panel4, 33);
    lv_obj_set_x(ui_Panel4, 75);
    lv_obj_set_y(ui_Panel4, -69);
    lv_obj_set_align(ui_Panel4, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Panel4, lv_color_hex(0x2995CD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_Panel4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Panel4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label15 = lv_label_create(ui_Panel4);
    lv_obj_set_width(ui_Label15, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label15, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label15, 10);
    lv_obj_set_y(ui_Label15, 1);
    lv_obj_set_align(ui_Label15, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label15, "本次得分: 45");
    lv_obj_set_style_text_font(ui_Label15, &ui_font_Font1, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Image2 = lv_img_create(ui_Panel4);
    lv_img_set_src(ui_Image2, &ui_img_score_png);
    lv_obj_set_width(ui_Image2, LV_SIZE_CONTENT);   /// 20
    lv_obj_set_height(ui_Image2, LV_SIZE_CONTENT);    /// 20
    lv_obj_set_x(ui_Image2, -49);
    lv_obj_set_y(ui_Image2, 1);
    lv_obj_set_align(ui_Image2, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image2, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_TextArea2 = lv_textarea_create(ui_Screen7);
    lv_obj_set_width(ui_TextArea2, 293);
    lv_obj_set_height(ui_TextArea2, 111);
    lv_obj_set_x(ui_TextArea2, 0);
    lv_obj_set_y(ui_TextArea2, 10);
    lv_obj_set_align(ui_TextArea2, LV_ALIGN_CENTER);
    lv_textarea_set_text(ui_TextArea2, "为您生成AI建议中...请耐心等待");
    lv_textarea_set_placeholder_text(ui_TextArea2, "Placeholder...");
    lv_obj_set_style_text_font(ui_TextArea2, &ui_font_Font1, LV_PART_MAIN | LV_STATE_DEFAULT);



    ui_Panel9 = lv_obj_create(ui_Screen7);
    lv_obj_set_width(ui_Panel9, 143);
    lv_obj_set_height(ui_Panel9, 33);
    lv_obj_set_x(ui_Panel9, -74);
    lv_obj_set_y(ui_Panel9, -69);
    lv_obj_set_align(ui_Panel9, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel9, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Panel9, lv_color_hex(0x298552), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_Panel9, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Panel9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label22 = lv_label_create(ui_Panel9);
    lv_obj_set_width(ui_Label22, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label22, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label22, 10);
    lv_obj_set_y(ui_Label22, 1);
    lv_obj_set_align(ui_Label22, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label22, "AI建议");
    lv_obj_set_style_text_font(ui_Label22, &ui_font_Font1, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Image3 = lv_img_create(ui_Screen7);
    lv_img_set_src(ui_Image3, &ui_img_ai_png);
    lv_obj_set_width(ui_Image3, LV_SIZE_CONTENT);   /// 20
    lv_obj_set_height(ui_Image3, LV_SIZE_CONTENT);    /// 20
    lv_obj_set_x(ui_Image3, -105);
    lv_obj_set_y(ui_Image3, -67);
    lv_obj_set_align(ui_Image3, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image3, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    lv_obj_add_event_cb(ui_Button5, ui_event_Button5, LV_EVENT_ALL, NULL);

    // 添加事件，当界面被加载时触发数据获取
    lv_obj_add_event_cb(ui_Screen7, on_screen_load, LV_EVENT_SCREEN_LOADED, NULL);
}
