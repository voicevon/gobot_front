#include "MyLibs/utility/logger.h"
#include "MyLibs/utility/web_configuator/web_configurator.h"
#include "MyLibs/mqtt/wifi_mqtt_client.h"
#include "board/board.h"
#include "board/web_configurator_diction.h"
#include "app_lua_wrapper.h"
#include "MyLibs/mqtt/remote_component/remote_var_chars.h"
#include "serial_port_sniffer_app.h"
#include "CNC/gcode/gcode_queue.h"

#include "../select_app.h"
#ifdef I_AM_SERIAL_PORT_SNIFFER


SerialPortSniffer_Board board;
WebConfiguratorDiction_SerialPortSniffer diction;
RemoteVar_Chars remote_lua_file;
SerialPortSniffer_App app;
// GcodeQueue gcode_queue;
TextMessageQueue command_queue;

void setup(){
	board.Init("I_AM_SERIAL_PORT_SNIFFER");

	// String line = String("print('Hello world!  Arduino -->Lua --> Arduino')");
	// String script = String("print('Hello world!  Arduino -->Lua --> Arduino')");


	// return;

	diction.Init();
	WebConfigurator::Begin(&diction);
	setup_wifi_mqtt_blocking_mode();  //TODO:  connect to wifi once.
	app.Link_Mqtt_to_CommandQueue("aaaaaaa", &command_queue);
	// remote_lua_file.InitFilename("/temp.lua");
	// lua.Begin("/temp.lua");

}

void loop(){
	// lua.SpinOnce();	
}

#endif
