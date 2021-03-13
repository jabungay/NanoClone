// wifi.h
// (c) Jonathan Bungay, 20210214

#include "globals.h"

#ifndef _WIFI_H
#define _WIFI_H

#include "wifi_auth.h"

#include "freertos/event_groups.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

void vWifiInit(void);
void vTaskWifi(void * pvParams);

#endif /* _WIFI_H */