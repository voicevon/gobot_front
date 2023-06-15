#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#define EVENT_BIT_MQTT_RX_0  (1 << 0)
#define EVENT_BIT_MQTT_RX_1  (1 << 1)
#define EVENT_BIT_MQTT_RX_2  (1 << 2)
#define EVENT_BIT_MQTT_RX_3  (1 << 3)
#define EVENT_BIT_MQTT_RX_4  (1 << 4)
#define EVENT_BIT_MQTT_RX_5  (1 << 5)
#define EVENT_BIT_MQTT_RX_6  (1 << 6)
#define EVENT_BIT_MQTT_RX_7  (1 << 7)
#define EVENT_BIT_MQTT_RX_8  (1 << 8)
#define EVENT_BIT_MQTT_RX_9  (1 << 9)
#define EVENT_BIT_MQTT_RX_10  (1 << 10)
#define EVENT_BIT_UART_MASTER_RX  (1 << 11)
#define EVENT_BIT_UART_SLAVE_RX  (1 << 12)


extern EventGroupHandle_t my_EventGroup;