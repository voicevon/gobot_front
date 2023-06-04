#include "MyLibs/utility/logger.h"
#include "MyLibs/utility/web_configuator/web_configurator.h"
#include "MyLibs/mqtt/wifi_mqtt_client.h"
#include "board/board.h"
#include "board/web_configurator_diction.h"
#include "app_lua_wrapper.h"

#include "../define_app.h"
#ifdef I_AM_SERIAL_PORET_SNIFFER

SerialPortSniffer_Board board;
WebConfiguratorDiction_SerialPortSniffer diction;
AppLuaWrapper lua;

void setup(){
	Serial.begin(115200);
	Logger::Info("I_AM_WIFI_MANAGER_DEMO");
	board.Init_SPIFFS();
	diction.Init();
	WebConfigurator::Begin(&diction);
	setup_wifi_mqtt_blocking_mode();  //TODO:  connect to wifi once.


	String script = String("print('Hello world!')");

	Serial.println(lua.Lua_dostring(&script));
}

void loop(){

}

#endif
