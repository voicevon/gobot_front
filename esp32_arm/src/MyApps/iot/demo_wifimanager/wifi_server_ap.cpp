#include "wifi_server_ap.h"

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "MyLibs/basic/logger.h"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Search for parameter in HTTP POST request
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "pass";
const char* PARAM_INPUT_3 = "admin_uid";
const char* PARAM_INPUT_4 = "admin_password";


//Variables to save values from HTML form


// File paths to save input values permanently

// Timer variables
unsigned long previousMillis = 0;
const long interval = 10000;  // interval to wait for Wi-Fi connection (milliseconds)

static String __ssidPath__ = "/ssid.txt";
static String __passPath__ = "/pass.txt";
static String __ipPath__ = "/ip.txt";
static String __gatewayPath__ = "/gateway.txt";

        static char __ssid[30];
        static char __pass[30];
        static char __admin_uid[30];
        static char __admin_password[30];

        static const char* ssidPath;
        static const char* passPath;
        static const char* ipPath;
        static const char* gatewayPath;


const char* WifiServerAp::GetSsid(){
    return __ssid;
}

const char* WifiServerAp::GetPassword(){
    return __pass;
}

void WifiServerAp::setup_callme(const char* file_name){
	initSPIFFS();
    ssidPath = __ssidPath__.c_str();
    passPath = __passPath__.c_str();
    ipPath = __ipPath__.c_str();
    gatewayPath = __gatewayPath__.c_str();
	// Set GPIO 2 as an OUTPUT
	// pinMode(ledPin, OUTPUT);
	// digitalWrite(ledPin, LOW);
    // static char ssidPath = xxx.c_str();
    // static char* passPath = String("/pass.txt").c_str();
    // static char[] ipPath = "/admin_uid.txt";
    // static char* gatewayPath = "/admin_password.txt";

	// Load values saved in SPIFFS
	readFile(SPIFFS, ssidPath).toCharArray(__ssid, 30);
	readFile(SPIFFS, passPath).toCharArray(__pass, 30);
	readFile(SPIFFS, ipPath).toCharArray(__admin_uid, 30);
	readFile (SPIFFS, gatewayPath).toCharArray(__admin_password,30);
	Serial.println(__ssid);
	Serial.println(__pass);
	Serial.println(__admin_uid);
	Serial.println(__admin_password);

	if(initWiFi()) {
		// setup_server();
	}
	else {
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
					p->value().toCharArray(__ssid,30);
					Serial.print("SSID set to: ");
					Serial.println(__ssid);
					// Write file to save value
					writeFile(SPIFFS, ssidPath, __ssid);
				}
				// HTTP POST pass value
				if (p->name() == PARAM_INPUT_2) {
					p->value().toCharArray(__pass,30);
					Serial.print("Password set to: ");
					Serial.println(__pass);
					// Write file to save value
					writeFile(SPIFFS, passPath, __pass);
				}
				// HTTP POST admin_uid value
				if (p->name() == PARAM_INPUT_3) {
					p->value().toCharArray(__admin_uid, 30);
					Serial.print("IP Address set to: ");
					Serial.println(__admin_uid);
					// Write file to save value
					writeFile(SPIFFS, ipPath, __admin_uid);
				}
				// HTTP POST admin_password value
				if (p->name() == PARAM_INPUT_4) {
					p->value().toCharArray(__admin_password, 30);
					Serial.print("Gateway set to: ");
					Serial.println(__admin_password);
					// Write file to save value
					writeFile(SPIFFS, gatewayPath, __admin_password);
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