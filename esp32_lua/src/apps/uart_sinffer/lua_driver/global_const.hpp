
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#define EVENT_BIT_UART_WIFI_TCP_RX  (1 << 2)


extern "C"{


    extern EventGroupHandle_t my_EventGroup;
}