

#include "lua_driver/task_master_uart.h"
#include "lua_driver/task_lua.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern "C"{
		//    void Task_LuaVM(void * parameter){}
	void Task_SlaveUart(void * parameter){

	}

	void Task_WebSocked(void * parameter){

	}



	void create_all_tasks(){
		// EventGroupHandle_t my_events = xEventGroupCreate();

		xTaskCreate(Task_MasterUart, /* Task function. */
		            "Master UART", /* name of task. */
		            10000, /* Stack size of task */
		            NULL, /* parameter of the task */
		            1, /* priority of the task */
		            NULL); 
		xTaskCreate(Task_SlaveUart, "Slave UART", 10000, NULL, 1, NULL); 
		xTaskCreate(Task_WebSocked, "Web socket", 10000, NULL, 1, NULL); 
		xTaskCreate(Task_LuaVM, "Lua-VM", 10000, NULL,  1,NULL);   // ??? stack size?
		
	}
}

void setup(){
	// serial_port_sniffer_board.Init("I_AM_SERIAL_PORT_SNIFFER");
	// // serial_port_sniffer_board.TestLeds();
	// // serial_port_sniffer_board.TestSerialPortMaster();
	// // serial_port_sniffer_board.TestSerialPortSlave();

	// diction.Init();
	// WebServerStarter::Begin(&diction);

	// // app.StartWebServer(&diction);
	// // setup_wifi_mqtt_blocking_mode();  //TODO:  connect to wifi once.
	// // app.Link_Mqtt_to_TextMessageQueue("serial_port_sniffer");
	// // app.Link_Lua(&lua_wrapper);
	// // // app.Link_lua_from_File(&lua_wrapper, "/test.lua");
	// // lua_wrapper.Link_Mqtt_for_Test("lua/test");
	create_all_tasks();
	// Logger::Info("Arduino setup() is done....");

}

void loop(){
	// app.SpinOnce();
	while(true){
		vTaskDelay(10000);
	}
}


