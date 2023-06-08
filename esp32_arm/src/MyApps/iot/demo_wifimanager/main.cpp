
	
#include <Arduino.h>
#include <WiFi.h>
#include "MyLibs/utility/webserver_starter/webserver_starter.h"
#include "web_configurator_diction_demo.h"
#include "MyLibs/utility/logger.h"
#include "Robot/board/board_base.h"
#include "MyLibs/mqtt/wifi_mqtt_client.h"

#include "../select_app.h"
#ifdef I_AM_WIFI_MANAGER_DEMO

DevBoard board;
WebConfiguratorDiction_Demo diction;




void setup(){
	board.Init("I_AM_WIFI_MANAGER_DEMO");
	diction.Init();
	WebConfigurator::Begin(&diction);
	setup_wifi_mqtt_blocking_mode();  //TODO:  connect to wifi once.


}

void loop() {


}


#endif