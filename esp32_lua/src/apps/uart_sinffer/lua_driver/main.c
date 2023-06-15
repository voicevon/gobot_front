

#include "task_master_uart.h"
#include "task_lua.h"
#include "task_slave_uart.h"
#include "task_wifi_tcp.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "von_clib/utility/logger/logger.h"


int __main(){
	InitMonitorUart();
	// InitRtos();
	// // Create 4 tasks
	// xTaskCreate(Task_MasterUart, /* Task function. */
	// 			"Master UART", /* name of task. */
	// 			10000, /* Stack size of task */
	// 			NULL, /* parameter of the task */
	// 			1, /* priority of the task */
	// 			NULL); 
	// xTaskCreate(Task_SlaveUart, "Slave UART", 10000, NULL, 1, NULL); 
	// xTaskCreate(Task_WebSocked, "Web socket", 10000, NULL, 1, NULL); 
	// xTaskCreate(Task_LuaVM, "Lua-VM", 10000, NULL,  1,NULL);   // ??? stack size?
}