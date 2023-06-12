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

extern "C"{
	#define BUF_SIZE (1024)
	// Both definition are same and valid
	// static intr_handle_t handle_console;


	static uart_dev_t *master_uart;
	static uart_port_t master_port;
	static uart_isr_handle_t *master_handle_console;
	uint8_t master_rx_buffer[256];  // Receive buffer to collect incoming data
	uint16_t master_rx_bytes_count;  // Register to collect data length
	// static MASTER_UART
	
	static uart_dev_t *slave_uart;
	static uart_port_t slave_port;
	static uart_isr_handle_t *slave_handle_console;
	uint8_t slave_rx_buffer[256];  // Receive buffer to collect incoming data
	uint16_t slave_rx_bytes_count;  // Register to collect data length

	/*
	* Define UART interrupt subroutine to ackowledge interrupt
	*/
	static void IRAM_ATTR master_uart_intr_handle(void *arg){
		uint16_t rx_fifo_len, status;
		uint16_t i;
		status = master_uart->int_st.val; // read UART interrupt Status
		rx_fifo_len = master_uart->status.rxfifo_cnt; // read number of bytes in UART buffer
		while(rx_fifo_len){
			master_rx_buffer[i++] = master_uart->fifo.rw_byte; // read all bytes
			rx_fifo_len--;
			master_rx_bytes_count++;
		}
		// after reading bytes from buffer clear UART interrupt status
		uart_clear_intr_status(master_port, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);
		Logger::Debug("Rx_Master");
	}

	static void IRAM_ATTR slave_uart_intr_handle(void *arg){
		uint16_t rx_fifo_len, status;
		uint16_t i;
		status = slave_uart->int_st.val; // read UART interrupt Status
		rx_fifo_len = slave_uart->status.rxfifo_cnt; // read number of bytes in UART buffer
		while(rx_fifo_len){
			slave_rx_buffer[i++] = slave_uart->fifo.rw_byte; // read all bytes
			rx_fifo_len--;
			slave_rx_bytes_count++;
		}
		// after reading bytes from buffer clear UART interrupt status
		uart_clear_intr_status(slave_port, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);
		Logger::Debug("Rx_Slave");
	}
	/*
	* main 
	*/

	// void __init_uart_with_isr(uart_port_t port, uart_config_t* uart_config, uint8_t pin_rx, uint8_t pin_tx, void()(void*) xxxx){
	// void __init_uart_with_isr(uart_port_t port, uart_config_t* uart_config, uint8_t pin_rx, uint8_t pin_tx, void()(void*) xxxx){
	// 	// esp_log_level_set(TAG, ESP_LOG_INFO);
	// 	ESP_ERROR_CHECK(uart_param_config(port, uart_config));
	// 	// esp_log_level_set(TAG, ESP_LOG_INFO);
	// 	ESP_ERROR_CHECK(uart_set_pin(port, pin_tx, pin_rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
	// 	ESP_ERROR_CHECK(uart_driver_install(port, BUF_SIZE * 2, 0, 0, NULL, 0)); //Install UART driver, and get the queue.
	// 	ESP_ERROR_CHECK(uart_isr_free(port)); // release the pre registered UART handler/subroutine
	// 	ESP_ERROR_CHECK(uart_isr_register(port, xxxx, NULL, ESP_INTR_FLAG_IRAM, &handle_console)); // register new UART subroutine
	// 	ESP_ERROR_CHECK(uart_enable_rx_intr(port)); // enable RX interrupt
	// }


	uart_dev_t *__GetUart_by_id(uart_port_t port){
		switch (port)
		{
		case 0:
			return &UART0;
			break;
		case 1:
			return &UART1;
			break;
		case 2:
			return &UART2;
			break;;
		default:
			Logger::Error("__GetUart_by_id()");
			break;
		}
	}
	
	void init_master_uart_with_isr(uart_port_t port, uart_config_t* uart_config, uint8_t pin_rx, uint8_t pin_tx) {
		master_uart = __GetUart_by_id(port);
		master_port = port;
		// __init_uart_with_isr(port, uart_config, pin_rx, pin_tx, master_uart_intr_handle);
		ESP_ERROR_CHECK(uart_param_config(port, uart_config));
		// esp_log_level_set(TAG, ESP_LOG_INFO);
		ESP_ERROR_CHECK(uart_set_pin(port, pin_tx, pin_rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
		ESP_ERROR_CHECK(uart_driver_install(port, BUF_SIZE * 2, 0, 0, NULL, 0)); //Install UART driver, and get the queue.
		ESP_ERROR_CHECK(uart_isr_free(port)); // release the pre registered UART handler/subroutine
		ESP_ERROR_CHECK(uart_isr_register(port, master_uart_intr_handle, NULL, ESP_INTR_FLAG_IRAM, master_handle_console)); // register new UART subroutine
		ESP_ERROR_CHECK(uart_enable_rx_intr(port)); // enable RX interrupt
	}

	void init_slave_uart_with_isr(uart_port_t port, uart_config_t* uart_config, uint8_t pin_rx, uint8_t pin_tx) {
		slave_uart = __GetUart_by_id(port);
		slave_port = port;
		// __init_uart_with_isr(port, uart_config,pin_rx, pin_tx, slave_uart_intr_handle);
		ESP_ERROR_CHECK(uart_param_config(port, uart_config));
		esp_log_level_set("SLAVE_INIT", ESP_LOG_INFO);
		ESP_ERROR_CHECK(uart_set_pin(port, pin_tx, pin_rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
		ESP_ERROR_CHECK(uart_driver_install(port, BUF_SIZE * 2, 0, 0, NULL, 0)); //Install UART driver, and get the queue.
		ESP_ERROR_CHECK(uart_isr_free(port)); // release the pre registered UART handler/subroutine
		ESP_ERROR_CHECK(uart_isr_register(port, slave_uart_intr_handle, NULL, ESP_INTR_FLAG_IRAM, slave_handle_console)); // register new UART subroutine
		ESP_ERROR_CHECK(uart_enable_rx_intr(port)); // enable RX interrupt
	}
}