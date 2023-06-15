#include <lua/lua.h>


// https://github.com/theElementZero/ESP32-UART-interrupt-handling/blob/master/uart_interrupt.c
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_intr_alloc.h"
#include "esp32/rom/uart.h"   //https://stackoverflow.com/questions/62453460/esp32-uart-interrupt


static int LuaUartRecv_Slave(lua_State* L){
	return 1;
}
static int LuaUartSend_Slave(lua_State* L){
	return 1;
}

static const char *TAG = "master_uart";

#define PIN_HARD_SERIAL_SLAVE_TX 25
#define PIN_HARD_SERIAL_SLAVE_RX 33

#define EX_UART_NUM UART_NUM_2
#define BUF_SIZE (1024)
#define RD_BUF_SIZE (BUF_SIZE)

static intr_handle_t handle_console;

static uint8_t rxbuf[256];
static uint16_t urxlen;
static int rx_packet_size = 0;




static void IRAM_ATTR uart_intr_handle(void *arg)
{
    uint16_t rx_fifo_len, status;
    uint16_t i;
    
    status = UART1.int_st.val; // read UART interrupt Status
    rx_fifo_len = UART1.status.rxfifo_cnt; // read number of bytes in UART buffer
    
    while(rx_fifo_len){
        rxbuf[i++] = UART1.fifo.rw_byte; // read all bytes
        rx_fifo_len--;
    }
    
    // after reading bytes from buffer clear UART interrupt status
    uart_clear_intr_status(EX_UART_NUM, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);

    // a test code or debug code to indicate UART receives successfully,
    // you can redirect received byte as echo also
    uart_write_bytes(EX_UART_NUM, (const char*) "RX Done", 7);

}
/*
 * main 
 */
static void init_isr()
{
	int ret;
	esp_log_level_set(TAG, ESP_LOG_INFO);

	/* Configure parameters of an UART driver,
	* communication pins and install the driver */
	uart_config_t uart_config = {
		.baud_rate = 115200,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE
	};

	ESP_ERROR_CHECK(uart_param_config(EX_UART_NUM, &uart_config));
	esp_log_level_set(TAG, ESP_LOG_INFO);
	ESP_ERROR_CHECK(uart_set_pin(EX_UART_NUM, PIN_HARD_SERIAL_SLAVE_TX, PIN_HARD_SERIAL_SLAVE_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
	ESP_ERROR_CHECK(uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0));
	ESP_ERROR_CHECK(uart_isr_free(EX_UART_NUM));
	ESP_ERROR_CHECK(uart_isr_register(EX_UART_NUM, uart_intr_handle, NULL, ESP_INTR_FLAG_IRAM, &handle_console));
	ESP_ERROR_CHECK(uart_enable_rx_intr(EX_UART_NUM));

}



static void __rx_buffer_to_package(){

}


void Task_SlaveUart(void * parameter){
    init_isr();

    while(true){
        __rx_buffer_to_package();
        if (rx_packet_size > 0 ){
            // set eventbit

            // suspend myself
        }
        vTaskDelay(1);
    }
}
