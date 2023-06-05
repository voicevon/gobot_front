#include "MyLibs/utility/logger.h"
#include "MyLibs/utility/web_configuator/web_configurator.h"
#include "MyLibs/mqtt/wifi_mqtt_client.h"
#include "board/board.h"
#include "board/web_configurator_diction.h"
#include "app_lua_wrapper.h"

#include "../select_app.h"
#ifdef I_AM_SERIAL_PORT_SNIFFER

SerialPortSniffer_Board board;
WebConfiguratorDiction_SerialPortSniffer diction;
AppLuaWrapper lua;

void setup(){
	board.Init();
	lua.Begin();
	String line = String("print('Hello world!  Arduino -->Lua --> Arduino')");
	// String script = String("print('Hello world!  Arduino -->Lua --> Arduino')");

	fs::File file = SPIFFS.open("/test.txt", FILE_READ);
	if (!file) {
		Serial.println("Failed to open file for reading");
		return;
	}
	while (file.available()) {
		String line = file.readStringUntil('\n');
		Logger::Info(line.c_str());
		String result = lua.Lua_dostring(&line);
		Serial.println(result);
	}
	file.close();

	return;

	diction.Init();
	WebConfigurator::Begin(&diction);
	setup_wifi_mqtt_blocking_mode();  //TODO:  connect to wifi once.


}

void loop(){

}

#endif
