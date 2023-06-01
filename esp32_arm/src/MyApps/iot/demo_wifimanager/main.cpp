
	
#include <Arduino.h>
#include <WiFi.h>
// #include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
// #include "SPIFFS.h"
#include "wifi_server_ap.h"



#include "all_applications.h"
#ifdef I_AM_WIFI_MANAGER_DEMO


void setup(){
	Serial.begin(115200);
	// WifiServerAp::Begin("/loop_porter.html", -1);
	WifiServerAp::setup_callme("/serial_port_sniffer.html", -1);

}

void loop() {


}


#endif