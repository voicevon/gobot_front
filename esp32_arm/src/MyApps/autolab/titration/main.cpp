#include "MyLibs/utility/logger.h"
#include "board/board.h"
#include "../select_app.h"
#ifdef I_AM_TITRATION_ROBOT

	
TiTrationBoard board;
WebConfiguratorDiction_Demo diction;

void setup(){
	Serial.begin(115200);
	Logger::Info("I_AM_WIFI_MANAGER_DEMO");
	board.Init_SPIFFS();
	diction.Init();
	WebServerStarter::Begin(&diction);
	setup_wifi_mqtt_blocking_mode();  //TODO:  connect to wifi once.


}


#endif
