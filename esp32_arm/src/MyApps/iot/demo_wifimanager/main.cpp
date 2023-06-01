
	
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "wifi_server_ap.h"
#include "MyLibs/basic/logger.h"


#include "all_applications.h"
#ifdef I_AM_WIFI_MANAGER_DEMO

// Search for parameter in HTTP POST request,  values are form.input.name

const char* html_name_ssid = "ssid";
const char* html_name_pass = "pass";
const char* html_name_admin_uid = "admin_uid";
const char* html_name_admin_password = "admin_password";
const char* html_form_item_names[4];

void setup(){
	Serial.begin(115200);
	Logger::Info("I_AM_WIFI_MANAGER_DEMO");
	// String ssid = "ssid";
	// String* all_items[4];
	html_form_item_names[0] = html_name_ssid;
	html_form_item_names[1] = html_name_pass;
	html_form_item_names[2] = html_name_admin_uid;
	html_form_item_names[3] = html_name_admin_password;
	


	Logger::Print("setup", html_name_ssid);
	Logger::Print("setup", html_form_item_names[0]);
	WifiServerAp::Begin("/loop_porter.html",&html_form_item_names[0], -1);

	WifiServerAp::Test();
	// WifiServerAp::Begin("/serial_port_sniffer.html", &html_form_item_names, -1);
	// WifiServerAp::GetConfig("/baud_rate");
}

void loop() {


}


#endif