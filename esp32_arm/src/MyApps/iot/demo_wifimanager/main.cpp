
	
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "wifi_server_ap.h"
#include "MyLibs/basic/logger.h"


#include "all_applications.h"
#ifdef I_AM_WIFI_MANAGER_DEMO

// Search for parameter in HTTP POST request,  values are form.input.name


void setup(){
	Serial.begin(115200);
	const char* PARAM_INPUT_1 = "ssid";
	const char* PARAM_INPUT_2 = "pass";
	const char* PARAM_INPUT_3 = "admin_uid";
	const char* PARAM_INPUT_4 = "admin_password";

	const char* html_form_item_names[4];
	html_form_item_names[0] = PARAM_INPUT_1;
	html_form_item_names[1] = PARAM_INPUT_2;
	html_form_item_names[2] = PARAM_INPUT_3;
	html_form_item_names[3] = PARAM_INPUT_4;

	Logger::Print("setup", PARAM_INPUT_1);
	Logger::Print("setup", html_form_item_names[0]);
	WifiServerAp::Begin("/loop_porter.html",&html_form_item_names[0], -1);

	// WifiServerAp::Begin("/serial_port_sniffer.html", &html_form_item_names, -1);
	// WifiServerAp::GetConfig("/baud_rate");
}

void loop() {


}


#endif