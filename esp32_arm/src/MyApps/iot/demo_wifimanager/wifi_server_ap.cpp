#include "wifi_server_ap.h"

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Search for parameter in HTTP POST request
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "pass";
const char* PARAM_INPUT_3 = "admin_uid";
const char* PARAM_INPUT_4 = "admin_password";


//Variables to save values from HTML form
String ssid;
String pass;
String admin_uid;
String admin_password;

// File paths to save input values permanently
const char* ssidPath = "/ssid.txt";
const char* passPath = "/pass.txt";
const char* ipPath = "/admin_uid.txt";
const char* gatewayPath = "/admin_password.txt";
// Timer variables
unsigned long previousMillis = 0;
const long interval = 10000;  // interval to wait for Wi-Fi connection (milliseconds)


void WifiServerAp::setup_callme(const char* file_name){
	initSPIFFS();

	// Set GPIO 2 as an OUTPUT
	// pinMode(ledPin, OUTPUT);
	// digitalWrite(ledPin, LOW);
	
	// Load values saved in SPIFFS
	ssid = readFile(SPIFFS, ssidPath);
	pass = readFile(SPIFFS, passPath);
	admin_uid = readFile(SPIFFS, ipPath);
	admin_password = readFile (SPIFFS, gatewayPath);
	Serial.println(ssid);
	Serial.println(pass);
	Serial.println(admin_uid);
	Serial.println(admin_password);

	if(initWiFi()) {
		// setup_server();
	}
	else {
		__instance.StartApServer();
	}
}

String WifiServerAp::GetSsid(){

}

String WifiServerAp::GetPassword(){

}

bool WifiServerAp::initWiFi() {
	if(ssid=="" || admin_uid==""){
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
	WiFi.begin(ssid.c_str(), pass.c_str());
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
		request->send(SPIFFS, "/wifimanager.html", "text/html");
	});
	
	server.serveStatic("/", SPIFFS, "/");
	
	server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
		int params = request->params();
		for(int i=0;i<params;i++){
			AsyncWebParameter* p = request->getParam(i);
			if(p->isPost()){
				// HTTP POST ssid value
				if (p->name() == PARAM_INPUT_1) {
					ssid = p->value().c_str();
					Serial.print("SSID set to: ");
					Serial.println(ssid);
					// Write file to save value
					__instance.writeFile(SPIFFS, ssidPath, ssid.c_str());
				}
				// HTTP POST pass value
				if (p->name() == PARAM_INPUT_2) {
					pass = p->value().c_str();
					Serial.print("Password set to: ");
					Serial.println(pass);
					// Write file to save value
					__instance.writeFile(SPIFFS, passPath, pass.c_str());
				}
				// HTTP POST admin_uid value
				if (p->name() == PARAM_INPUT_3) {
					admin_uid = p->value().c_str();
					Serial.print("IP Address set to: ");
					Serial.println(admin_uid);
					// Write file to save value
					writeFile(SPIFFS, ipPath, admin_uid.c_str());
				}
				// HTTP POST admin_password value
				if (p->name() == PARAM_INPUT_4) {
					admin_password = p->value().c_str();
					Serial.print("Gateway set to: ");
					Serial.println(admin_password);
					// Write file to save value
					writeFile(SPIFFS, gatewayPath, admin_password.c_str());
				}
				//Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
			}
		}
		request->send(200, "text/plain", "Done. ESP will restart, connect to your router and go to IP address: " + admin_uid);
		delay(3000);
		ESP.restart();
	});
	server.begin();
}