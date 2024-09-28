#ifndef TASK_FLAGS_H
#define TASK_FLAGS_H

#include <stdbool.h>  // 确保布尔类型的使用
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

// 声明任务运行状态的标志
extern volatile bool max30102_task_running;
extern volatile bool camera_task_running;
extern volatile bool audio_task_running;

// 定义事件组标志位
#define TASK_START_BIT (1 << 0)

// 声明任务事件组
extern EventGroupHandle_t task_event_group;

#endif // TASK_FLAGS_H
