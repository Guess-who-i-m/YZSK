// task_flags.c
#include "task_flags.h"
#include <stdbool.h> // 添加这行

volatile bool max30102_task_running = false;
volatile bool camera_task_running = false;
volatile bool audio_task_running = false;
