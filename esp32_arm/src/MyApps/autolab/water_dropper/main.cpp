#include "MyLibs/utility/logger.h"
#include "MyLibs/utility/web_configuator/web_configurator.h"
#include "MyLibs/mqtt/wifi_mqtt_client.h"
#include "MyLibs/mqtt/remote_component/remote_var_chars.h"

#include "board/board.h"
#include "board/web_configurator_diction.h"
#include "app_lua_wrapper.h"
#include "app.h"
#include "CNC/gcode/gcode_queue.h"

#include "../select_app.h"
#ifdef I_AM_WATER_DROPPER


WaterDropper_Board board;

WaterDropper_WebConfiguratorDiction diction;
WaterDropper_App app;
GcodeQueue gcode_queue;
TextMessageQueue command_queue;

WaterDropper_LuaWrapper lua_wrapper;
// RemoteVar_Chars remote_lua_file;

#define PIN_ENCODER_A 23
#define PIN_ENCODER_B 24

Encoder encoder = Encoder(PIN_ENCODER_A, PIN_ENCODER_B, 20 );
void doA(){encoder.handleA();}
void doB(){encoder.handleB();}
void init_encoder(){
	encoder.quadrature = Quadrature::ON;
	encoder.pullup = Pullup::USE_INTERN;
	encoder.init();
	encoder.enableInterrupts(doA, doB);
}

void setup(){
	board.Init("I_AM_WATER_DROPPER");
	// init_encoder();
	// board.LinkEncoder(&encoder);

	diction.Init(board.Get_ConfigButton());
	WebConfigurator::Begin(&diction);
	// setup_wifi_mqtt_blocking_mode();  //TODO:  connect to wifi once.
	// app.Link_Mqtt_to_CommandQueue("aaaaaaa", &command_queue);
	lua_wrapper.Begin("/test.lua");

}

void loop(){
	lua_wrapper.SpinOnce();	
	// encoder.update();
	// float aa = encoder.getAngle();
	// Logger::Print("encoder.angle", aa);
}

#endif
