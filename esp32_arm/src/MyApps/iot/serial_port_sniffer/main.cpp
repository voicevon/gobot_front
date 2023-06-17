#include "von/utility/logger.h"
#include "MyLibs/utility/webserver_starter/webserver_starter.h"
// #include "von/utility/mqtt/task_mqtt.h"
// #include "MyLibs/mqtt/remote_component/remote_var_chars.h"
#include "board/board.h"
#include "board/web_configurator_diction.h"
// #include "lua/app_lua.h"
// #include "app.h"

#include "../select_app.h"
#ifdef I_AM_SERIAL_PORT_SNIFFER


WebConfiguratorDiction_SerialPortSniffer diction;
// RemoteVar_Chars remote_lua_file;
// SerialPortSniffer_App app;
// SerialPortSniffer_LuaWrapper lua_wrapper;


void __rx_buffer_to_package(){

}

void Task_MasterUart(void * parameter){
    // if (rx_packet_queue.size() > 0){
    //     myevents.setbit();
    //     self.suspend();
    // }
    __rx_buffer_to_package();
}

void Task_SlaveUart(void * parameter){

}

void Task_WebSocked(void * parameter){

}

void Task_LuaVM(void * parameter){

}

void create_all_tasks(){
	EventGroupHandle_t my_events = xEventGroupCreate();

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


void setup(){
	serial_port_sniffer_board.Init("I_AM_SERIAL_PORT_SNIFFER");
	// serial_port_sniffer_board.TestLeds();
	// serial_port_sniffer_board.TestSerialPortMaster();
	// serial_port_sniffer_board.TestSerialPortSlave();

	diction.Init();
	WebServerStarter::Begin(&diction);

	// app.StartWebServer(&diction);
	// setup_wifi_mqtt_blocking_mode();  //TODO:  connect to wifi once.
	// app.Link_Mqtt_to_TextMessageQueue("serial_port_sniffer");
	// app.Link_Lua(&lua_wrapper);
	// // app.Link_lua_from_File(&lua_wrapper, "/test.lua");
	// lua_wrapper.Link_Mqtt_for_Test("lua/test");
	create_all_tasks();
	Logger::Info("Arduino setup() is done....");

}

void loop(){
	// app.SpinOnce();
}

#endif
