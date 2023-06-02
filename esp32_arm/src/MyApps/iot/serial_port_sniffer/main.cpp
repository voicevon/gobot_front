#include "MyLibs/utility/logger.h"

#include "all_applications.h"
#ifdef I_AM_SERIAL_PORET_SNIFFER
	const char* PARAM_INPUT_1 = "ssid";
	const char* PARAM_INPUT_2 = "pass";
	const char* PARAM_INPUT_3 = "admin_uid";
	const char* PARAM_INPUT_4 = "admin_password";

	const char* html_form_item_names[4];
	
void setup(){
	Serial.begin(115200);
    Logger::Info("I_AM_SERIAL_PORET_SNIFFER");
    

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

void loop(){

}


#endif
