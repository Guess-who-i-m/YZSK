#ifndef _APP_MAX30102_H_
#define _APP_MAX30102_H_
#include "esp_err.h"

static esp_err_t i2c_init();
void max30102_update_task(void *pvParam);
// void app_max30102_init(void);
void i2c_scan();

#endif
