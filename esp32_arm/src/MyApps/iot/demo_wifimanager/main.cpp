
	
#include <Arduino.h>
#include <WiFi.h>
#include "MyLibs/utility/web_configuator/web_configurator.h"
#include "web_configurator_diction_demo.h"
#include "MyLibs/utility/logger.h"
#include "Robot/board/board_base.h"
#include "MyLibs/mqtt/wifi_mqtt_client.h"

#include "../select_app.h"
#ifdef I_AM_WIFI_MANAGER_DEMO

DevBoard board;
WebConfiguratorDiction_Demo diction;

void setup(){
	Serial.begin(115200);
	Logger::Info("I_AM_WIFI_MANAGER_DEMO");
	board.Init_SPIFFS();
	diction.Init();
	WebConfigurator::Begin(&diction);
	setup_wifi_mqtt_blocking_mode();  //TODO:  connect to wifi once.


}

void loop() {


}


#endif