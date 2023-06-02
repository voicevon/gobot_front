
// https://RandomNerdTutorials.com/esp32-wi-fi-manager-asyncwebserver/
#include "web_configurator.h"

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "MyLibs/utility/logger.h"
#include "base/web-configurator_parameter.h"
#include "SPIFFS.h"
#include "esp_wifi.h"


AsyncWebServer ap_webserver(80);  // Create AsyncWebServer object on port 80
static WebConfigurator_DictionBase* diction;
static WebConnfigurator_Parameter para_of_any_key = WebConnfigurator_Parameter();

// Timer variables
static unsigned long previousMillis = 0;
static const long interval = 20000;  // interval to wait for Wi-Fi connection (milliseconds)

const char* WebConfigurator::GetSsid(){
    return diction->para_wifi_ssid.GetName();
}

const char* WebConfigurator::GetPassword(){
    return diction->para_wifi_pass.GetName();
}

const char* WebConfigurator::GetConfig(const char* key){
	para_of_any_key.SetName(key);
	return para_of_any_key.readFile();
}

void WebConfigurator::Begin(WebConfigurator_DictionBase* web_configurator_diction){
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

bool WebConfigurator::initWiFi() {
	if(diction->para_wifi_ssid.readFile()==0x00 || diction->para_wifi_pass.readFile()==0x00){
		Serial.println("Undefined SSID or IP address.");
		return false;
	}

	WiFi.mode(WIFI_STA);
    Logger::Print("connectToWifi()  point", 2);
    WiFi.disconnect();       //disconnect from an AP if it was previously connected     
    Logger::Print("connectToWifi()  point", 3);
    ESP_ERROR_CHECK(esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_11B |WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N));

	const char* ssid = diction->para_wifi_ssid.readFile();
	const char* pass= diction->para_wifi_pass.readFile();

	// Logger::Debug("WebConfigurator::initWiFi()" );
	// Serial.print("-->");
	// Serial.print(ssid);
	// Serial.print("<----->" );
	// Serial.print(pass);
	// Serial.println("<--");

	WiFi.begin(ssid, pass);


	Logger::Print ("Connecting to WiFi...  torlerance time(in millis second) is ", interval);

	unsigned long currentMillis = millis();
	previousMillis = currentMillis;

	while(WiFi.status() != WL_CONNECTED) {
		currentMillis = millis();
		if (currentMillis - previousMillis >= interval) {
			Logger::Print("WebConfigurator::initWiFi()  Timeout", "Failed to connect Wifi-AP");
			return false;
		}
	}

	Logger::Print("WebConfigurator::initWiFi()  Connected", WiFi.localIP().toString().c_str());
	return true;
}

// Initialize SPIFFS
void WebConfigurator::initSPIFFS() {
	if (!SPIFFS.begin(true)) {
		Serial.println("An error has occurred while mounting SPIFFS");
	}
	Serial.println("SPIFFS mounted successfully");
}


void WebConfigurator::StartApServer(){
	// Connect to Wi-Fi network with SSID and password
	Serial.println("Setting AP (Access Point)");
	// NULL sets an open Access Point
	WiFi.softAP("Integral-Setup", NULL);

	IPAddress IP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(IP); 

	// Web Server Root URL
	ap_webserver.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
		Logger::Info("webserver. on  /");
		Logger::Print("parameter_name", diction->HtmlFilename_of_Configurator);
		request->send(SPIFFS, diction->HtmlFilename_of_Configurator, "text/html");
	});
	
	ap_webserver.serveStatic("/", SPIFFS, "/");
	
	ap_webserver.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
		int params = request->params();
		// Logger::Debug("WebConfigurator::StartApServer()  HTTP-POST");
		// Logger::Print("???????????????????", HTML_FORM_ITEM_NAMES[0]);
		for(int i=0;i<params;i++){
			AsyncWebParameter* p = request->getParam(i);
			if(p->isPost()){
				WebConnfigurator_Parameter* para = diction->FindItem(p->name().c_str());
				// item-> Link_AsyncWebParameter(p);
				para-> WriteToFile(p);

			}
		}
        String strAdminUid = diction->para_admin_uid.GetName();
		request->send(200, "text/plain", "Done. ESP will restart, connect to your router and go to IP address: " + strAdminUid);
		delay(3000);
		ESP.restart();
	});
	ap_webserver.begin();
}