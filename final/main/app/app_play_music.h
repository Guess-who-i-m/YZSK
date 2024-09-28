/* SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void app_music_player_init(void);
void music_play_pause(void);
void music_play_resume(void);
bool is_music_playing(void);

#ifdef __cplusplus
}
#endif