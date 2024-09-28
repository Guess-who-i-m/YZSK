/* SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <dirent.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "bsp/esp-bsp.h"
#include "audio_player.h"
#include "file_iterator.h"
#include "bsp_board_extra.h"

#include "app_play_music.h"


static const char *TAG = "app_play_music";

static file_iterator_instance_t *file_iterator;

// 判断文件是不是mp3
static bool isMP3(const char* filename) {
    const char* dot = strrchr(filename, '.');
    if (dot && dot != filename) {
        if (strcasecmp(dot, ".mp3") == 0) {
            return true;
        }
    }
    return false;
}

static void play_index(int index)
{
    ESP_LOGI(TAG, "play_index(%d)", index);

    char filename[128];
    int retval = file_iterator_get_full_path_from_index(file_iterator, index, filename, sizeof(filename));

    const char* name = file_iterator_get_name_from_index(file_iterator, index);
    // lv_label_set_text(ui_MusicName, name);

    if (retval == 0) {
        ESP_LOGE(TAG, "unable to retrieve filename");
        return;
    }

    // 这是用来播放音乐的
    FILE *fp = fopen(filename, "rb");
    if (fp) {
        audio_player_play(fp);
    } else {
        ESP_LOGE(TAG, "unable to open index %d, filename '%s'", index, filename);
    }
}

// 滑钮设定音量
// void volume_slider_cb(lv_event_t *event)
// {
//     lv_obj_t *slider = (lv_obj_t *) event->target;
//     int volume = lv_slider_get_value(slider);
//     bsp_extra_codec_volume_set(volume, NULL);
//     ESP_LOGI(TAG, "volume '%d'", volume);
// }

// void music_forward(lv_event_t * e)
// {
//     file_iterator_next(file_iterator);

//     audio_player_state_t state = audio_player_get_state();
//     if (state == AUDIO_PLAYER_STATE_IDLE) {
//         /* Nothing to do, doesn't start or stop playback */
//         ESP_LOGI(TAG, "idle, nothing to do");
//     } else if (state == AUDIO_PLAYER_STATE_PLAYING) {
//         int index = file_iterator_get_index(file_iterator);
//         play_index(index);
//     }
// }

// void music_backward(lv_event_t * e)
// {
//     file_iterator_prev(file_iterator);

//     audio_player_state_t state = audio_player_get_state();
//     if (state == AUDIO_PLAYER_STATE_IDLE) {
//         /* Nothing to do, doesn't start or stop playback */
//         ESP_LOGI(TAG, "idle, nothing to do");
//     } else if (state == AUDIO_PLAYER_STATE_PLAYING) {
//         int index = file_iterator_get_index(file_iterator);
//         play_index(index);
//     }
// }

// void music_play(lv_event_t * e)
// {
//     audio_player_state_t state = audio_player_get_state();
//     if (state == AUDIO_PLAYER_STATE_PLAYING) {
//         /* If user want to use pause/resume,
//            replace 'audio_player_stop()' with 'audio_player_pause()' */
//         audio_player_stop();
//     } else if (state == AUDIO_PLAYER_STATE_IDLE) {
//         int index = file_iterator_get_index(file_iterator);
//         play_index(index);
//     } else if (state == AUDIO_PLAYER_STATE_PAUSE) {
//         audio_player_resume();
//     }
// }

bool is_music_playing(void)
{
    if (audio_player_get_state() == AUDIO_PLAYER_STATE_PLAYING) {
        return true;
    } else {
        return false;
    }
}

void music_play_pause(void)
{
    if (audio_player_get_state() == AUDIO_PLAYER_STATE_PLAYING) {
        /* If user want to use pause/resume,
           replace 'audio_player_stop()' with 'audio_player_pause()' */
        audio_player_stop();
    }
}

void music_play_resume(void)
{
    if (audio_player_get_state() == AUDIO_PLAYER_STATE_IDLE) {
        int index = file_iterator_get_index(file_iterator);
        play_index(index);
    } else if (audio_player_get_state() == AUDIO_PLAYER_STATE_PAUSE) {
        audio_player_resume();
    }
}


void app_music_player_init(void)
{
    file_iterator = file_iterator_new(BSP_SPIFFS_MOUNT_POINT);
    assert(file_iterator != NULL);
}
