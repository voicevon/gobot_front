#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#define EVENT_BIT_UART_MASTER_RX  (1 << 0)
#define EVENT_BIT_UART_SLAVE_RX  (1 << 1)
#define EVENT_BIT_UART_WIFI_TCP_RX  (1 << 2)


extern EventGroupHandle_t my_EventGroup;