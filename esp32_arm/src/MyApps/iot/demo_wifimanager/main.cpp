// https://RandomNerdTutorials.com/esp32-wi-fi-manager-asyncwebserver/
	
#include <Arduino.h>
#include <WiFi.h>
// #include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
// #include "SPIFFS.h"
#include "wifi_server_ap.h"



#include "all_applications.h"
#ifdef I_AM_WIFI_MANAGER_DEMO
// Create AsyncWebServer object on port 80
// AsyncWebServer server(80);

// Search for parameter in HTTP POST request
// const char* PARAM_INPUT_1 = "ssid";
// const char* PARAM_INPUT_2 = "pass";
// const char* PARAM_INPUT_3 = "admin_uid";
// const char* PARAM_INPUT_4 = "admin_password";


//Variables to save values from HTML form
// String ssid;
// String pass;
// String admin_uid;
// String admin_password;

// File paths to save input values permanently
// const char* ssidPath = "/ssid.txt";
// const char* passPath = "/pass.txt";
// const char* ipPath = "/admin_uid.txt";
// const char* gatewayPath = "/admin_password.txt";

IPAddress localIP;
//IPAddress localIP(192, 168, 1, 200); // hardcoded

// Set your Gateway IP address
IPAddress localGateway;
//IPAddress localGateway(192, 168, 1, 1); //hardcoded
IPAddress subnet(255, 255, 0, 0);


// Set LED GPIO
const int ledPin = 2;
// Stores LED state

String ledState;

// // Initialize SPIFFS
// void initSPIFFS() {
// 	if (!SPIFFS.begin(true)) {
// 		Serial.println("An error has occurred while mounting SPIFFS");
// 	}
// 	Serial.println("SPIFFS mounted successfully");
// }

// // Read File from SPIFFS
// String readFile(fs::FS &fs, const char * path){
// 	Serial.printf("Reading file: %s\r\n", path);

// 	File file = fs.open(path);
// 	if(!file || file.isDirectory()){
// 		Serial.println("- failed to open file for reading");
// 		return String();
// 	}
	
// 	String fileContent;
// 	while(file.available()){
// 		fileContent = file.readStringUntil('\n');
// 		break;     
// 	}
// 	return fileContent;
// }

// // Write file to SPIFFS
// void writeFile(fs::FS &fs, const char * path, const char * message){
// 	Serial.printf("Writing file: %s\r\n", path);

// 	File file = fs.open(path, FILE_WRITE);
// 	if(!file){
// 		Serial.println("- failed to open file for writing");
// 		return;
// 	}
// 	if(file.print(message)){
// 		Serial.println("- file written");
// 	} else {
// 		Serial.println("- write failed");
// 	}
// }

// Initialize WiFi
// bool initWiFi() {
// 	if(ssid=="" || admin_uid==""){
// 		Serial.println("Undefined SSID or IP address.");
// 		return false;
// 	}

// 	WiFi.mode(WIFI_STA);
// 	// localIP.fromString(admin_uid.c_str());
// 	// localGateway.fromString(admin_password.c_str());


// 	// if (!WiFi.config(localIP, localGateway, subnet)){
// 	//   Serial.println("STA Failed to configure");
// 	//   return false;
// 	// }
// 	WiFi.begin(ssid.c_str(), pass.c_str());
// 	Serial.println("Connecting to WiFi...");

// 	unsigned long currentMillis = millis();
// 	previousMillis = currentMillis;

// 	while(WiFi.status() != WL_CONNECTED) {
// 		currentMillis = millis();
// 		if (currentMillis - previousMillis >= interval) {
// 			Serial.println("Failed to connect.");
// 			return false;
// 		}
// 	}

// 	Serial.println(WiFi.localIP());
// 	return true;
// }

// Replaces placeholder with LED state value
String processor(const String& var) {
	if(var == "STATE") {
		if(digitalRead(ledPin)) {
			ledState = "ON";
		}
		else {
			ledState = "OFF";
		}
		return ledState;
	}
	return String();
}

// void setup_server(){
// 	// Route for root / web page
//     server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
//       request->send(SPIFFS, "/index.html", "text/html", false, processor);
//     });
//     server.serveStatic("/", SPIFFS, "/");
    
//     // Route to set GPIO state to HIGH
//     server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request) {
//       digitalWrite(ledPin, HIGH);
//       request->send(SPIFFS, "/index.html", "text/html", false, processor);
//     });

//     // Route to set GPIO state to LOW
//     server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {
//       digitalWrite(ledPin, LOW);
//       request->send(SPIFFS, "/index.html", "text/html", false, processor);
//     });
//     server.begin();
// }

// void setup_ap_server(){

// }

void setup(){
	// Serial port for debugging purposes
	Serial.begin(115200);
	WifiServerAp::setup_callme("loop_porter.html");

}

void loop() {
	String xxx="";
	// char[] xx = xxx.c_str();

}


#endif