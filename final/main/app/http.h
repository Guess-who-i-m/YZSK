#include <string.h>
#include <sys/param.h>
#include <stdlib.h>
#include <ctype.h>
#include "esp_log.h"
#include "esp_event.h"
#include "protocol_examples_common.h"
#include "protocol_examples_utils.h"
#include "esp_tls.h"


#include "esp_system.h"

#include "esp_http_client.h"
#include "cJSON.h"

esp_err_t _http_event_handler(esp_http_client_event_t *evt);
char* http_rest_with_url(void);
void set_session_id(void);