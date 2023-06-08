#include "MyLibs/utility/logger.h"
#include "MyLibs/utility/webserver_starter/webserver_starter.h"
#include "MyLibs/mqtt/wifi_mqtt_client.h"
#include "MyLibs/mqtt/remote_component/remote_var_chars.h"
#include "board/board.h"
#include "board/web_configurator_diction.h"
#include "app_lua_wrapper.h"
#include "serial_port_sniffer_app.h"

#include "../select_app.h"
#ifdef I_AM_SERIAL_PORT_SNIFFER


// SerialPortSniffer_Board serial_port_sniffer_board;
WebConfiguratorDiction_SerialPortSniffer diction;
RemoteVar_Chars remote_lua_file;
SerialPortSniffer_App app;
SerialPortSniffer_LuaWrapper lua_wrapper;

void setup(){
	serial_port_sniffer_board.Init("I_AM_SERIAL_PORT_SNIFFER");
	serial_port_sniffer_board.TestLeds();
	serial_port_sniffer_board.TestSerialPortMaster();
	serial_port_sniffer_board.TestSerialPortSlave();



	diction.Init();
	WebServerStarter::Begin(&diction);
	setup_wifi_mqtt_blocking_mode();  //TODO:  connect to wifi once.

	app.Link_Mqtt_to_TextMessageQueue("serial_port_sniffer");
	app.Link_lua_from_File(&lua_wrapper, "/test.lua");
	Logger::Info("Arduino setup() is done....");

}

void loop(){
	app.SpinOnce();
}

#endif
