
// https://RandomNerdTutorials.com/esp32-wi-fi-manager-asyncwebserver/
#include "wifi_server_ap.h"

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "MyLibs/basic/logger.h"

#define MAX_STRING_LENGTH_IN_HTML_INPUT 30
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Search for parameter in HTTP POST request,  values are form.input.name
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "pass";
const char* PARAM_INPUT_3 = "admin_uid";
const char* PARAM_INPUT_4 = "admin_password";



// File paths to save input values permanently
static String FILE_ssid = "/ssid.txt";
static String FILE_password = "/pass.txt";
static String FILE_admin_uid = "/admin_uid.txt";
static String FILE_admin_password = "/admin_password.txt";
static String __html_filename = "";

// Timer variables
unsigned long previousMillis = 0;
const long interval = 10000;  // interval to wait for Wi-Fi connection (milliseconds)

        static  char __ssid[MAX_STRING_LENGTH_IN_HTML_INPUT];
        static  char __pass[MAX_STRING_LENGTH_IN_HTML_INPUT];
        static  char __admin_uid[MAX_STRING_LENGTH_IN_HTML_INPUT];
        static  char __admin_password[MAX_STRING_LENGTH_IN_HTML_INPUT];
        static  char __value[MAX_STRING_LENGTH_IN_HTML_INPUT];
const char* WifiServerAp::GetSsid(){
    return __ssid;
}

const char* WifiServerAp::GetPassword(){
    return __pass;
}

const char* WifiServerAp::GetConfig(const char* key){
	readFile(SPIFFS, key).toCharArray(__value, MAX_STRING_LENGTH_IN_HTML_INPUT);
    return __value;
}

void WifiServerAp::Begin(String html_filename, int8_t gpio_of_config_button){
    __html_filename = html_filename;
	bool is_workstation_mode = true;
	if (gpio_of_config_button > 0){
		pinMode(gpio_of_config_button, INPUT_PULLUP);
		delay(100);
		if (digitalRead(gpio_of_config_button) == LOW){
			// use config-button, but the button is not pushed
			is_workstation_mode = false;
		}
	}
	initSPIFFS();
	
	readFile(SPIFFS, FILE_ssid.c_str()).toCharArray(__ssid, MAX_STRING_LENGTH_IN_HTML_INPUT);
	readFile(SPIFFS, FILE_password.c_str()).toCharArray(__pass, MAX_STRING_LENGTH_IN_HTML_INPUT);
	readFile(SPIFFS, FILE_admin_uid.c_str()).toCharArray(__admin_uid, MAX_STRING_LENGTH_IN_HTML_INPUT);
	readFile (SPIFFS, FILE_admin_password.c_str()).toCharArray(__admin_password,MAX_STRING_LENGTH_IN_HTML_INPUT);
	// Serial.println(__ssid);
	// Serial.println(__pass);
	// Serial.println(__admin_uid);
	// Serial.println(__admin_password);

	if (is_workstation_mode){
		if(!initWiFi()) {
			is_workstation_mode = false;
	}
	if (!is_workstation_mode)
		StartApServer();
	}
}

bool WifiServerAp::initWiFi() {
	if(__ssid=="" || __admin_uid==""){
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
	WiFi.begin(__ssid, __pass);
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
void WifiServerAp::initSPIFFS() {
	if (!SPIFFS.begin(true)) {
		Serial.println("An error has occurred while mounting SPIFFS");
	}
	Serial.println("SPIFFS mounted successfully");
}

// Read File from SPIFFS
String WifiServerAp::readFile(fs::FS &fs, const char * path){
	Serial.printf("Reading file: %s\r\n", path);

	File file = fs.open(path);
	if(!file || file.isDirectory()){
		Serial.println("- failed to open file for reading");
		return String();
	}
	
	String fileContent;
	while(file.available()){
		fileContent = file.readStringUntil('\n');
		break;     
	}
	return fileContent;
}

// Write file to SPIFFS
void WifiServerAp::writeFile(fs::FS &fs, const char * path, const char * message){
	Serial.printf("Writing file: %s\r\n", path);

	File file = fs.open(path, FILE_WRITE);
	if(!file){
		Serial.println("- failed to open file for writing");
		return;
	}
	if(file.print(message)){
		Serial.println("- file written");
	} else {
		Serial.println("- write failed");
	}
}

void WifiServerAp::StartApServer(){
	// Connect to Wi-Fi network with SSID and password
	Serial.println("Setting AP (Access Point)");
	// NULL sets an open Access Point
	WiFi.softAP("Integral-Setup", NULL);

	IPAddress IP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(IP); 

	// Web Server Root URL
	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(SPIFFS, __html_filename, "text/html");
	});
	
	server.serveStatic("/", SPIFFS, "/");
	
	server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
		int params = request->params();
		for(int i=0;i<params;i++){
			AsyncWebParameter* p = request->getParam(i);
			if(p->isPost()){
				// HTTP POST ssid value
				if (p->name() == PARAM_INPUT_1) {
					p->value().toCharArray(__ssid,MAX_STRING_LENGTH_IN_HTML_INPUT);
					Serial.print("SSID set to: ");
					Serial.println(__ssid);
					// Write file to save value
					writeFile(SPIFFS, FILE_ssid.c_str(), __ssid);
				}
				// HTTP POST pass value
				if (p->name() == PARAM_INPUT_2) {
					p->value().toCharArray(__pass,MAX_STRING_LENGTH_IN_HTML_INPUT);
					Serial.print("Password set to: ");
					Serial.println(__pass);
					// Write file to save value
					writeFile(SPIFFS, FILE_password.c_str(), __pass);
				}
				// HTTP POST admin_uid value
				if (p->name() == PARAM_INPUT_3) {
					p->value().toCharArray(__admin_uid, MAX_STRING_LENGTH_IN_HTML_INPUT);
					Serial.print("IP Address set to: ");
					Serial.println(__admin_uid);
					// Write file to save value
					writeFile(SPIFFS, FILE_admin_uid.c_str(), __admin_uid);
				}
				// HTTP POST admin_password value
				if (p->name() == PARAM_INPUT_4) {
					p->value().toCharArray(__admin_password, MAX_STRING_LENGTH_IN_HTML_INPUT);
					Serial.print("Gateway set to: ");
					Serial.println(__admin_password);
					// Write file to save value
					writeFile(SPIFFS, FILE_admin_password.c_str(), __admin_password);
				}
				//Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
			}
		}
        String strAdminUid = __admin_uid;
		request->send(200, "text/plain", "Done. ESP will restart, connect to your router and go to IP address: " + strAdminUid);
		delay(3000);
		ESP.restart();
	});
	server.begin();
}