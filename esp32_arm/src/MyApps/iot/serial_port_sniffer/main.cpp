#include "MyLibs/utility/logger.h"
#include "MyLibs/utility/web_configuator/web_configurator.h"
#include "MyLibs/mqtt/wifi_mqtt_client.h"
#include "board/board.h"
#include "board/web_configurator_diction.h"

#include "all_applications.h"
#ifdef I_AM_SERIAL_PORET_SNIFFER

SerialPortSniffer_Board board;
WebConfiguratorDiction_SerialPortSniffer diction;

void setup(){
	Serial.begin(115200);
	Logger::Info("I_AM_WIFI_MANAGER_DEMO");
	board.Init_SPIFFS();
	diction.Init();
	WebConfigurator::Begin(&diction);
	setup_wifi_mqtt_blocking_mode();  //TODO:  connect to wifi once.
}

void loop(){

}


#endif
