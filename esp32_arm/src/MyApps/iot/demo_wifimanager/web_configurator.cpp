
// https://RandomNerdTutorials.com/esp32-wi-fi-manager-asyncwebserver/
#include "web_configurator.h"

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "MyLibs/basic/logger.h"
#include "web-configurator_parameter.h"
#include "SPIFFS.h"

#define MAX_STRING_LENGTH_IN_HTML_INPUT 30

AsyncWebServer ap_webserver(80);  // Create AsyncWebServer object on port 80
static const char** HTML_FORM_ITEM_NAMES;
static WebConfigurator_DictionBase* diction;

// File paths to save input values permanently
// static String FILE_ssid = "/ssid.txt";
// static String FILE_password = "/pass.txt";
static String FILE_admin_uid = "/admin_uid.txt";
static String FILE_admin_password = "/admin_password.txt";
static String __html_filename = "";
static WebConnfigurator_Parameter para_of_key = WebConnfigurator_Parameter();


// Timer variables
static unsigned long previousMillis = 0;
static const long interval = 10000;  // interval to wait for Wi-Fi connection (milliseconds)
// static char __ssid[MAX_STRING_LENGTH_IN_HTML_INPUT];
// static char __pass[MAX_STRING_LENGTH_IN_HTML_INPUT];
// static char __admin_uid[MAX_STRING_LENGTH_IN_HTML_INPUT];
// static char __admin_password[MAX_STRING_LENGTH_IN_HTML_INPUT];
static char __value[MAX_STRING_LENGTH_IN_HTML_INPUT];

const char* WebConfiturator::GetSsid(){
    return diction->__ssid.GetName();
}

const char* WebConfiturator::GetPassword(){
    return diction->__ssid.GetName();
}

const char* WebConfiturator::GetConfig(const char* key){
	para_of_key.SetName(key);
	return para_of_key.readFile();
}

void WebConfiturator::Begin(WebConfigurator_DictionBase* web_configurator_diction){
	diction = web_configurator_diction;
	// __html_filename = web_configurator_diction->HtmlFilename_of_Configurator;
	// int8_t gpio_of_config_button= web_configurator_diction->Gpio_of_ConfigButton;
	// HTML_FORM_ITEM_NAMES = web_configurator_diction->Items;
	// Logger::Print("begin()", HTML_FORM_ITEM_NAMES[0]);
	bool is_workstation_mode = true;
	if (diction->Gpio_of_ConfigButton > 0){
		pinMode(diction->Gpio_of_ConfigButton, INPUT_PULLUP);
		delay(100);
		if (digitalRead(diction->Gpio_of_ConfigButton) == LOW){
			// use config-button, but the button is not pushed
			is_workstation_mode = false;
		}
	}
	initSPIFFS();
	
	// readFile(SPIFFS, FILE_ssid.c_str()).toCharArray(__ssid, MAX_STRING_LENGTH_IN_HTML_INPUT);
	// readFile(SPIFFS, FILE_password.c_str()).toCharArray(__pass, MAX_STRING_LENGTH_IN_HTML_INPUT);
	// readFile(SPIFFS, FILE_admin_uid.c_str()).toCharArray(__admin_uid, MAX_STRING_LENGTH_IN_HTML_INPUT);
	// readFile (SPIFFS, FILE_admin_password.c_str()).toCharArray(__admin_password,MAX_STRING_LENGTH_IN_HTML_INPUT);
	Serial.println(diction->__ssid.readFile());
	Serial.println(diction->__pass.readFile());
	Serial.println(diction->__admin_uid.readFile());
	Serial.println(diction->__admin_password.readFile());

	if (is_workstation_mode){
		if(!initWiFi()) {
			is_workstation_mode = false;
	}
	if (!is_workstation_mode)
		StartApServer();
	}
}

// void WebConfiturator::Begin(String html_filename, const char** html_form_item_names, int8_t gpio_of_config_button){
//     __html_filename = html_filename;
// 	HTML_FORM_ITEM_NAMES = html_form_item_names;
// 	// Logger::Print("begin()", HTML_FORM_ITEM_NAMES[0]);
// 	bool is_workstation_mode = true;
// 	if (gpio_of_config_button > 0){
// 		pinMode(gpio_of_config_button, INPUT_PULLUP);
// 		delay(100);
// 		if (digitalRead(gpio_of_config_button) == LOW){
// 			// use config-button, but the button is not pushed
// 			is_workstation_mode = false;
// 		}
// 	}
// 	initSPIFFS();
	
// 	readFile(SPIFFS, FILE_ssid.c_str()).toCharArray(__ssid, MAX_STRING_LENGTH_IN_HTML_INPUT);
// 	readFile(SPIFFS, FILE_password.c_str()).toCharArray(__pass, MAX_STRING_LENGTH_IN_HTML_INPUT);
// 	readFile(SPIFFS, FILE_admin_uid.c_str()).toCharArray(__admin_uid, MAX_STRING_LENGTH_IN_HTML_INPUT);
// 	readFile (SPIFFS, FILE_admin_password.c_str()).toCharArray(__admin_password,MAX_STRING_LENGTH_IN_HTML_INPUT);
// 	Serial.println(__ssid);
// 	Serial.println(__pass);
// 	Serial.println(__admin_uid);
// 	Serial.println(__admin_password);

// 	if (is_workstation_mode){
// 		if(!initWiFi()) {
// 			is_workstation_mode = false;
// 	}
// 	if (!is_workstation_mode)
// 		StartApServer();
// 	}
// }

bool WebConfiturator::initWiFi() {
	if(diction->__ssid.readFile()==0x00 || diction->__pass.readFile()==0x00){
		Serial.println("Undefined SSID or IP address.");
		return false;
	}

	WiFi.mode(WIFI_STA);
	// localIP.fromString(admin_uid.c_str());
	// localGateway.fromString(admin_password.c_str());


	// if (!WiFi.config(localIP, localGateway, subnet)){
	//   Serial.println("STA Failed to configure");
	//   return false;
	// }
	WiFi.begin(diction->__ssid.readFile(), diction->__pass.readFile());
	Serial.println("Connecting to WiFi...");

	unsigned long currentMillis = millis();
	previousMillis = currentMillis;

	while(WiFi.status() != WL_CONNECTED) {
		currentMillis = millis();
		if (currentMillis - previousMillis >= interval) {
			Serial.println("Failed to connect.");
			return false;
		}
	}

	Serial.println(WiFi.localIP());
	return true;
}

// Initialize SPIFFS
void WebConfiturator::initSPIFFS() {
	if (!SPIFFS.begin(true)) {
		Serial.println("An error has occurred while mounting SPIFFS");
	}
	Serial.println("SPIFFS mounted successfully");
}


void WebConfiturator::StartApServer(){
	// Connect to Wi-Fi network with SSID and password
	Serial.println("Setting AP (Access Point)");
	// NULL sets an open Access Point
	WiFi.softAP("Integral-Setup", NULL);

	IPAddress IP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(IP); 

	// Web Server Root URL
	ap_webserver.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(SPIFFS, __html_filename, "text/html");
	});
	
	ap_webserver.serveStatic("/", SPIFFS, "/");
	
	ap_webserver.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
		int params = request->params();
		// Logger::Debug("WebConfiturator::StartApServer()  HTTP-POST");
		// Logger::Print("???????????????????", HTML_FORM_ITEM_NAMES[0]);
		for(int i=0;i<params;i++){
			AsyncWebParameter* p = request->getParam(i);
			if(p->isPost()){
				WebConnfigurator_Parameter* item = diction->FindItem(p->name().c_str());
				// item-> Link_AsyncWebParameter(p);
				item-> WriteToFile(p);
				// p->value().toCharArray(__ssid,MAX_STRING_LENGTH_IN_HTML_INPUT);
				// writeFile(SPIFFS, item->GetSpiffsFilename, __ssid);

				
				// HTTP POST ssid value
				// Logger::Print("p->name()",HTML_FORM_ITEM_NAMES[0]);
				// if (p->name() == HTML_FORM_ITEM_NAMES[0]) {
				// 	// Logger::Print("point",111);
				// 	p->value().toCharArray(__ssid,MAX_STRING_LENGTH_IN_HTML_INPUT);
				// 	Serial.print("SSID set to: ");
				// 	Serial.println(__ssid);
				// 	// Write file to save value
				// 	writeFile(SPIFFS, FILE_ssid.c_str(), __ssid);
				// }
				// // HTTP POST pass value
				// if (p->name() == HTML_FORM_ITEM_NAMES[1]) {
				// 	p->value().toCharArray(__pass,MAX_STRING_LENGTH_IN_HTML_INPUT);
				// 	Serial.print("Password set to: ");
				// 	Serial.println(__pass);
				// 	// Write file to save value
				// 	writeFile(SPIFFS, FILE_password.c_str(), __pass);
				// }
				// // HTTP POST admin_uid value
				// if (p->name() == HTML_FORM_ITEM_NAMES[2]) {
				// 	p->value().toCharArray(__admin_uid, MAX_STRING_LENGTH_IN_HTML_INPUT);
				// 	Serial.print("IP Address set to: ");
				// 	Serial.println(__admin_uid);
				// 	// Write file to save value
				// 	writeFile(SPIFFS, FILE_admin_uid.c_str(), __admin_uid);
				// }
				// // HTTP POST admin_password value
				// if (p->name() == HTML_FORM_ITEM_NAMES[3]) {
				// 	p->value().toCharArray(__admin_password, MAX_STRING_LENGTH_IN_HTML_INPUT);
				// 	Serial.print("Gateway set to: ");
				// 	Serial.println(__admin_password);
				// 	// Write file to save value
				// 	writeFile(SPIFFS, FILE_admin_password.c_str(), __admin_password);
				// }
				// //Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
			}
		}
        String strAdminUid = diction->__admin_uid.GetName();
		request->send(200, "text/plain", "Done. ESP will restart, connect to your router and go to IP address: " + strAdminUid);
		delay(3000);
		ESP.restart();
	});
	ap_webserver.begin();
}