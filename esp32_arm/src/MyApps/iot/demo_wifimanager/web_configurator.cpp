
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
static WebConfigurator_DictionBase* diction;


// static String __html_filename = "";
static WebConnfigurator_Parameter para_of_any_key = WebConnfigurator_Parameter();


// Timer variables
static unsigned long previousMillis = 0;
static const long interval = 10000;  // interval to wait for Wi-Fi connection (milliseconds)

const char* WebConfiturator::GetSsid(){
    return diction->para_wifi_ssid.GetName();
}

const char* WebConfiturator::GetPassword(){
    return diction->para_wifi_pass.GetName();
}

const char* WebConfiturator::GetConfig(const char* key){
	para_of_any_key.SetName(key);
	return para_of_any_key.readFile();
}

void WebConfiturator::Begin(WebConfigurator_DictionBase* web_configurator_diction){
	diction = web_configurator_diction;
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
	
	Serial.println(diction->para_wifi_ssid.readFile());
	Serial.println(diction->para_wifi_pass.readFile());
	Serial.println(diction->para_admin_uid.readFile());
	Serial.println(diction->para_admin_password.readFile());

	if (is_workstation_mode){
		if(!initWiFi()) {
			is_workstation_mode = false;
	}
	if (!is_workstation_mode)
		StartApServer();
	}
}

bool WebConfiturator::initWiFi() {
	if(diction->para_wifi_ssid.readFile()==0x00 || diction->para_wifi_pass.readFile()==0x00){
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
	WiFi.begin(diction->para_wifi_ssid.readFile(), diction->para_wifi_pass.readFile());
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
		request->send(SPIFFS, diction->HtmlFilename_of_Configurator, "text/html");
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
			}
		}
        String strAdminUid = diction->para_admin_uid.GetName();
		request->send(200, "text/plain", "Done. ESP will restart, connect to your router and go to IP address: " + strAdminUid);
		delay(3000);
		ESP.restart();
	});
	ap_webserver.begin();
}