
	
#include <Arduino.h>
#include <WiFi.h>
#include "MyLibs/utility/web_configuator/web_configurator.h"
#include "web_configurator_diction_demo.h"
#include "MyLibs/utility/logger.h"


#include "all_applications.h"
#ifdef I_AM_WIFI_MANAGER_DEMO

WebConfiguratorDiction_Demo diction;

void setup(){
	Serial.begin(115200);
	Logger::Info("I_AM_WIFI_MANAGER_DEMO");
	diction.Init();
	WebConfiturator::Begin(&diction);
}

void loop() {


}


#endif