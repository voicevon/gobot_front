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
	// #define MASTER_UART UART_NUM_1
	#define MASTER_UART UART_NUM_1
	#define SLAVE_UART UART_NUM_2
	// #define PATTERN_CHR_NUM    (3)         /*!< Set the number of consecutive and identical characters received by receiver which defines a UART pattern*/

	#define BUF_SIZE (1024)
	// #define RD_BUF_SIZE (BUF_SIZE)
	// static QueueHandle_t uart0_queue;

	// Both definition are same and valid
	//static uart_isr_handle_t *handle_console;
	static intr_handle_t handle_console;


	uint8_t master_rx_buffer[256];  // Receive buffer to collect incoming data
	uint16_t master_rx_bytes_count;  // Register to collect data length
	// static MASTER_UART

	// #define NOTASK 0

	// void blink_task(void *pvParameter)
	// {
	// 	gpio_pad_select_gpio(BLINK_GPIO);
		
	// 	/* Set the GPIO as a push/pull output */
	// 	gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
		
	// 	while(1) {
	// 		/* Blink off (output low) */
	// 		gpio_set_level(BLINK_GPIO, 0);
	// 		vTaskDelay(1000 / portTICK_PERIOD_MS);
	// 		/* Blink on (output high) */
	// 		gpio_set_level(BLINK_GPIO, 1);
	// 		vTaskDelay(1000 / portTICK_PERIOD_MS);
	// 	}
	// }
	/*
	* Define UART interrupt subroutine to ackowledge interrupt
	*/
	static void IRAM_ATTR master_uart_intr_handle(void *arg){
		uint16_t rx_fifo_len, status;
		uint16_t i;
		status = UART1.int_st.val; // read UART interrupt Status
		rx_fifo_len = UART1.status.rxfifo_cnt; // read number of bytes in UART buffer
		while(rx_fifo_len){
			master_rx_buffer[i++] = UART1.fifo.rw_byte; // read all bytes
			rx_fifo_len--;
			master_rx_bytes_count++;
		}
		// after reading bytes from buffer clear UART interrupt status
		uart_clear_intr_status(MASTER_UART, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);
	}

	static void IRAM_ATTR slave_uart_intr_handle(void *arg){
		uint16_t rx_fifo_len, status;
		uint16_t i;
		status = UART1.int_st.val; // read UART interrupt Status
		rx_fifo_len = UART1.status.rxfifo_cnt; // read number of bytes in UART buffer
		while(rx_fifo_len){
			master_rx_buffer[i++] = UART1.fifo.rw_byte; // read all bytes
			rx_fifo_len--;
			master_rx_bytes_count++;
		}
		// after reading bytes from buffer clear UART interrupt status
		uart_clear_intr_status(MASTER_UART, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);
	}
	/*
	* main 
	*/

	// void __init_uart_with_isr(uart_port_t port, uart_config_t* uart_config, uint8_t pin_rx, uint8_t pin_tx, void()(void*) xxxx){
	// void __init_uart_with_isr(uart_port_t port, uart_config_t* uart_config, uint8_t pin_rx, uint8_t pin_tx, void()(void*)* xxxx){
	// 	int ret;
	// 	// esp_log_level_set(TAG, ESP_LOG_INFO);
	// 	ESP_ERROR_CHECK(uart_param_config(port, uart_config));
	// 	// esp_log_level_set(TAG, ESP_LOG_INFO);
	// 	ESP_ERROR_CHECK(uart_set_pin(port, pin_tx, pin_rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
	// 	ESP_ERROR_CHECK(uart_driver_install(port, BUF_SIZE * 2, 0, 0, NULL, 0)); //Install UART driver, and get the queue.
	// 	ESP_ERROR_CHECK(uart_isr_free(port)); // release the pre registered UART handler/subroutine
	// 	ESP_ERROR_CHECK(uart_isr_register(port, xxxx, NULL, ESP_INTR_FLAG_IRAM, &handle_console)); // register new UART subroutine
	// 	ESP_ERROR_CHECK(uart_enable_rx_intr(port)); // enable RX interrupt
	// }

	void init_master_uart_with_isr(uart_port_t port, uart_config_t* uart_config, uint8_t pin_rx, uint8_t pin_tx) {
		// __init_uart_with_isr(port, uart_config, pin_rx, pin_tx, &master_uart_intr_handle);
	}

	void init_slave_uart_with_isr(uart_port_t port, uart_config_t* uart_config, uint8_t pin_rx, uint8_t pin_tx) {
		// __init_uart_with_isr(port, uart_config,pin_rx, pin_tx, &slave_uart_intr_handle);
	}
}