#pragma once

/* 
https://github.com/theElementZero/ESP32-UART-interrupt-handling/blob/master/uart_interrupt.c
*/
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
#include "esp32/rom/uart.h"


//  Master has 5 wires
//TODO:  rename to HARD
#define PIN_HARD_SERIAL_MASTER_TX 17
#define PIN_HARD_SERIAL_MASTER_RX 16

// Slave has 3 wires
#define PIN_HARD_SERIAL_SLAVE_TX 25
#define PIN_HARD_SERIAL_SLAVE_RX 33

#define BLINK_GPIO GPIO_NUM_2

static const char *TAG = "uart_events";

/**
 * This example shows how to use the UART driver to handle UART interrupt.
 *
 * - Port: UART0
 * - Receive (Rx) buffer: on
 * - Transmit (Tx) buffer: off
 * - Flow control: off
 * - Event queue: on
 * - Pin assignment: TxD (default), RxD (default)
 */

extern "C"{
#define EX_UART_NUM UART_NUM_1
#define PATTERN_CHR_NUM    (3)         /*!< Set the number of consecutive and identical characters received by receiver which defines a UART pattern*/

#define BUF_SIZE (1024)
// #define RD_BUF_SIZE (BUF_SIZE)
// static QueueHandle_t uart0_queue;

// Both definition are same and valid
//static uart_isr_handle_t *handle_console;
static intr_handle_t handle_console;


uint8_t rxbuf[256];  // Receive buffer to collect incoming data
uint16_t urxlen;  // Register to collect data length

// #define NOTASK 0

void blink_task(void *pvParameter)
{
	gpio_pad_select_gpio(BLINK_GPIO);
	
	/* Set the GPIO as a push/pull output */
	gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
	
	while(1) {
		/* Blink off (output low) */
		gpio_set_level(BLINK_GPIO, 0);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		/* Blink on (output high) */
		gpio_set_level(BLINK_GPIO, 1);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}
/*
* Define UART interrupt subroutine to ackowledge interrupt
*/
static void IRAM_ATTR master_uart_intr_handle(void *arg){
	uint16_t rx_fifo_len, status;
	uint16_t i;

	status = UART1.int_st.val; // read UART interrupt Status
	rx_fifo_len = UART1.status.rxfifo_cnt; // read number of bytes in UART buffer

	while(rx_fifo_len){
		rxbuf[i++] = UART1.fifo.rw_byte; // read all bytes
		rx_fifo_len--;
        urxlen++;
	}

	// after reading bytes from buffer clear UART interrupt status
	uart_clear_intr_status(EX_UART_NUM, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);

	// a test code or debug code to indicate UART receives successfully,
	// you can redirect received byte as echo also
	// uart_write_bytes(0, (const char*) "RX Done", 7);
}
/*
* main 
*/
// void task_starter_uart_sniffer() {
void init_uart_with_isr() {
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
	//Set UART log level
	esp_log_level_set(TAG, ESP_LOG_INFO);
	//Set UART pins (using UART0 default pins ie no changes.)
	ESP_ERROR_CHECK(uart_set_pin(EX_UART_NUM, PIN_HARD_SERIAL_MASTER_TX, PIN_HARD_SERIAL_MASTER_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
	//Install UART driver, and get the queue.
	ESP_ERROR_CHECK(uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0));
	// release the pre registered UART handler/subroutine
	ESP_ERROR_CHECK(uart_isr_free(EX_UART_NUM));
	// register new UART subroutine
	ESP_ERROR_CHECK(uart_isr_register(EX_UART_NUM, master_uart_intr_handle, NULL, ESP_INTR_FLAG_IRAM, &handle_console));
	// enable RX interrupt
	ESP_ERROR_CHECK(uart_enable_rx_intr(EX_UART_NUM));
	
	// #if (NOTASK == 1)
	// while(1){
	// 	vTaskDelay(1000);
	// }
	// #else
	// 	xTaskCreate(&blink_task, "blink_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
	// #endif
}
}