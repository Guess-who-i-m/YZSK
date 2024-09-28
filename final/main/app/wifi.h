#include "esp_wifi.h"
#include <esp_log.h>
#include "freertos/event_groups.h"

#define EXAMPLE_ESP_WIFI_SSID      "qianyu"
#define EXAMPLE_ESP_WIFI_PASS      "04150415"
#define EXAMPLE_ESP_WIFI_CHANNEL   1
#define EXAMPLE_MAX_STA_CONN       4

static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void wifi_init_sta(void);

