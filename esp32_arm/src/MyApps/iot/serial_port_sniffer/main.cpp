#include "MyLibs/utility/logger.h"
#include "MyLibs/utility/webserver_starter/webserver_starter.h"
#include "MyLibs/mqtt/wifi_mqtt_client.h"
#include "MyLibs/mqtt/remote_component/remote_var_chars.h"
#include "board/board.h"
#include "board/web_configurator_diction.h"
#include "lua/app_lua.h"
#include "app.h"

#include "../select_app.h"
#ifdef I_AM_SERIAL_PORT_SNIFFER


WebConfiguratorDiction_SerialPortSniffer diction;
RemoteVar_Chars remote_lua_file;
SerialPortSniffer_App app;
SerialPortSniffer_LuaWrapper lua_wrapper;

void setup(){
	serial_port_sniffer_board.Init("I_AM_SERIAL_PORT_SNIFFER");
	// serial_port_sniffer_board.TestLeds();
	// serial_port_sniffer_board.TestSerialPortMaster();
	// serial_port_sniffer_board.TestSerialPortSlave();

	diction.Init();
	//TODO:/.   lua_init_diction();
	app.StartWebServer(&diction);
	setup_wifi_mqtt_blocking_mode();  //TODO:  connect to wifi once.
	app.Link_Mqtt_to_TextMessageQueue("serial_port_sniffer");
	app.Link_Lua(&lua_wrapper);
	// app.Link_lua_from_File(&lua_wrapper, "/test.lua");
	lua_wrapper.Link_Mqtt_for_Test("lua/test");
	Logger::Info("Arduino setup() is done....");

}

void loop(){
	app.SpinOnce();
}

#endif
