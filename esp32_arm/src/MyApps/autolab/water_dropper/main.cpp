#include "von/utility/logger.h"
#include "board/web_configurator_diction.h"
#include "MyLibs/utility/webserver_starter/webserver_starter.h"
#include "von/utility/mqtt/g_var.h"
#include "von/utility/mqtt/task_mqtt.h"
#include "von/utility/wifi/task_wifi.h"


#include "board/board.h"
#include "lua/app_lua.h"
#include "app.h"

#include "../select_app.h"
#ifdef I_AM_WATER_DROPPER

WaterDropper_App app;
WaterDropper_LuaWrapper lua_wrapper;

#define PIN_ENCODER_A 16
#define PIN_ENCODER_B 17

Encoder encoder = Encoder(PIN_ENCODER_A, PIN_ENCODER_B, 20 );
void doA(){encoder.handleA();}
void doB(){encoder.handleB();}
void init_encoder(){
	encoder.quadrature = Quadrature::ON;
	encoder.pullup = Pullup::USE_INTERN;
	encoder.init();
	encoder.enableInterrupts(doA, doB);
}


#define IS_DEBUGING_LUA 
// TaskHandle_t task_Mqtt;

void setup(){

	init_encoder();
	water_dropper_board.Init("I_AM_WATER_DROPPER");
	water_dropper_board.LinkEncoder(&encoder);

	water_dropper_webconfigurator_diction.Init(water_dropper_board.Get_WebConfigButton());
	app.StartWebServer(&water_dropper_webconfigurator_diction);
	// return;
	#ifdef IS_DEBUGING_LUA
		// setup_wifi_mqtt_blocking_mode();  //TODO:  connect to wifi once.
		xTaskCreate(TaskMqtt, "Mqtt", 10000, NULL,  1, &task_Mqtt);   
		ConnectToWifi_FakeTask();
		app.Init();
		app.Link_Lua(&lua_wrapper);
		lua_wrapper.Link_Mqtt_for_Test("lua/test");
	#else
		app.Init();
		app.Link_Lua(&lua_wrapper);
		app.Lua_DoFile(&lua_wrapper, "/water_dropper.lua");
	#endif
	// app.Link_Mqtt_to_TextMessageQueue("water_dropper/command");
	Logger::Info("I_AM_WATER_DROPPER setup is done...");
}

void loop(){
	water_dropper_board.SpinOnce();
	app.SpinOnce();
}

#endif
