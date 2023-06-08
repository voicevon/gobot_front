
// https://RandomNerdTutorials.com/esp32-wi-fi-manager-asyncwebserver/
// #include "web_configurator.h"
#include "webserver_starter.h"

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "MyLibs/utility/logger.h"
#include "base/ap_webserver_diction_base.h"
#include "esp_wifi.h"

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta charset="UTF-8">
</head>
<body>
  <p><h1>File Upload</h1></p>
  <p>Free Storage: %FREESPIFFS% | Used Storage: %USEDSPIFFS% | Total Storage: %TOTALSPIFFS%</p>
  <form method="POST" action="/upload" enctype="multipart/form-data"><input type="file" name="data"/><input type="submit" name="upload" value="Upload" title="Upload File"></form>
  <p>After clicking upload it will take some time for the file to firstly upload and then be written to SPIFFS, there is no indicator that the upload began.  Please be patient.</p>
  <p>Once uploaded the page will refresh and the newly uploaded file will appear in the file list.</p>
  <p>If a file does not appear, it will be because the file was too big, or had unusual characters in the file name (like spaces).</p>
  <p>You can see the progress of the upload by watching the serial output.</p>
  <p>%FILELIST%</p>
</body>
</html>
)rawliteral";

// static WebServerStarter::Config config;                        // configuration


AsyncWebServer webserver(80);  // Create AsyncWebServer object on port 80
static ApWebserver_DictionBase* diction;
static Html_Parameter para_of_any_key = Html_Parameter();
static WebServerStarter::EnumState state;


// Timer variables
static unsigned long previousMillis = 0;
static const long interval = 10000;  // interval to wait for Wi-Fi connection (milliseconds)

const char* WebServerStarter::GetSsid(){
    return diction->para_wifi_ssid.GetName();
}

const char* WebServerStarter::GetPassword(){
    return diction->para_wifi_pass.GetName();
}

const char* WebServerStarter::GetConfig(const char* key){
	para_of_any_key.SetName(key);
	return para_of_any_key.readFile();
}


bool WebServerStarter::__statemachine_spinonce(){
	bool want_ap_mode = false;
	// Html_Parameter* para_ssid = diction->FindItem("ssid");
	// Html_Parameter* para_try_sta = diction->FindItem("try_sta");
	//************************************************************
	if (state == WebServerStarter::EnumState::IDLE){
		if (diction->ConfigButton != nullptr){
			if (diction->ConfigButton->IsFired()){
				want_ap_mode = true;
			}
		}else{
			Logger::Warn("dcition->ConfigButton is not linkded.....");
		}
		if (diction->para_wifi_ssid.ReadFile()->IsEqualTo("") or diction->para_wifi_ssid.ReadFile()->IsEqualTo("SSID")){
			want_ap_mode = true;
		}
		if (want_ap_mode){
			__EnterWifiApMode();
			state = WebServerStarter::EnumState::AS_AP;
			return false;
		}else{
			if (__Connect_to_a_Router()){
				// successed , return, will go-on with sta.
				return true;
			}else{
				// failed
				__EnterWifiApMode();
				state = WebServerStarter::EnumState::AS_AP;
				return false;
			}
		}
	}
	//**************************************************************
	if (state == WebServerStarter::EnumState::AS_AP){
		// webserver event post will set to ADMIN_DONE
		return false;
	}
	if (state == WebServerStarter::EnumState::ADMIN_DONE){
		if (diction->para_try_sta.ReadFile()->IsEqualTo("false")){
			// restart esp
			delay(3000);
			ESP.restart();
		}else{
			// return,will  go on with ap
			return true;
		}
	}
}

void WebServerStarter::Begin(ApWebserver_DictionBase* web_configurator_diction, bool webserver_on_ap_only){
	Logger::Info("WebServerStarter::Begin()");
	diction = web_configurator_diction;
	bool is_ended=false;
	state = WebServerStarter::EnumState::IDLE;
	while (!is_ended){

		WebServerStarter::__statemachine_spinonce();
	}

	Logger::Info("WebServerStarter is exiting....");
}

bool WebServerStarter::__Connect_to_a_Router() {
	if(diction->para_wifi_ssid.readFile()==0x00 || diction->para_wifi_pass.readFile()==0x00){
		Serial.println("Undefined SSID or IP address.");
		return false;
	}
	WiFi.mode(WIFI_STA);
    ESP_ERROR_CHECK(esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_11B |WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N));

	const char* ssid = diction->para_wifi_ssid.readFile();
	const char* pass= diction->para_wifi_pass.readFile();
	Logger::Debug("WebServerStarter::__connect_to_a_router()");
	Logger::Print(ssid, pass);
	WiFi.begin(ssid, pass);
	// WiFi.begin("Perfect", "1234567890");
	Logger::Print ("Connecting to WiFi...  torlerance time(in millis second) is ", interval);

	unsigned long currentMillis = millis();
	previousMillis = currentMillis;

	while(WiFi.status() != WL_CONNECTED) {
		currentMillis = millis();
		if (currentMillis - previousMillis >= interval) {
			Logger::Print("WebServerStarter::initWiFi()  Timeout", "Failed to connect Wifi-AP");
			return false;
		}
	}

	Logger::Print("WebServerStarter::initWiFi()  Connected", WiFi.localIP().toString().c_str());
	return true;
}

void WebServerStarter::__InitWebServer() {
	webserver.on("/files", HTTP_GET, [](AsyncWebServerRequest * request) {
		String logmessage = "Client:" + request->client()->remoteIP().toString() + + " " + request->url();
		Serial.println(logmessage);
		request->send_P(200, "text/html", index_html, processor_upload_file);
	});

	// run handleUpload function when any file is uploaded
	webserver.on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) {
			request->send(200);
		}, handleUpload);

	// Web Server Root URL
	webserver.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
		Logger::Info("webserver. on  /");
		Logger::Print("parameter_name", diction->HtmlFilename_of_Configurator->GetChars());
		request->send(SPIFFS, diction->HtmlFilename_of_Configurator->GetChars(), "text/html");
	});
	
	webserver.serveStatic("/", SPIFFS, "/");
	
	webserver.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
		int params = request->params();
		// Logger::Debug("WebServerStarter::StartApServer()  HTTP-POST");
		// Logger::Print("???????????????????", HTML_FORM_ITEM_NAMES[0]);
		for(int i=0;i<params;i++){
			AsyncWebParameter* p = request->getParam(i);
			if(p->isPost()){
				Html_Parameter* para = diction->FindItem(p->name().c_str());
				// item-> Link_AsyncWebParameter(p);
				para-> WriteToFile(p);
			}
		}
		state = WebServerStarter::EnumState::ADMIN_DONE;
		request->send(200, "text/plain", "Done. ESP might be restart: ");
	});
}


void WebServerStarter::__EnterWifiApMode(){
		Logger::Info("Entering AP(Access Point) mode, ssid name='Integral-Setup'");
		WiFi.softAP("Integral-Setup", NULL);
		// IPAddress IP = WiFi.softAPIP();
		Logger::Print("AP IP address: ", WiFi.softAPIP().toString().c_str());

// 	// Serial.println(IP); 
}


void WebServerStarter::__StartLuaEditor(){
	// Web Server Root URL
	webserver.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
		Logger::Info("webserver. on /   is a lua_scrpt editor ");
		request->send(SPIFFS, "/lua_editor.html", "text/html");
	});
	webserver.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
		int params = request->params();
		// Logger::Debug("WebServerStarter::StartApServer()  HTTP-POST");
		// Logger::Print("???????????????????", HTML_FORM_ITEM_NAMES[0]);
		for(int i=0;i<params;i++){
			AsyncWebParameter* p = request->getParam(i);
			if(p->isPost()){
				Html_Parameter* para = diction->FindItem(p->name().c_str());
				// item-> Link_AsyncWebParameter(p);
				para-> WriteToFile(p);
				Logger::Info("Lua script is updated,  Will restart Lua-Virtual machine");

			}
		}
        // String strAdminUid = diction->para_admin_uid.GetName();
		// request->send(200, "text/plain", "Done. ESP will restart, connect to your router and go to IP address: " + strAdminUid);
		// delay(3000);
		// ESP.restart();
	});
	webserver.begin();

}

// list all of the files, if ishtml=true, return html rather than simple text
String WebServerStarter::listFiles(bool ishtml) {
  String returnText = "";
  Serial.println("Listing files stored on SPIFFS");
  File root = SPIFFS.open("/");
  File foundfile = root.openNextFile();
  if (ishtml) {
    returnText += "<table><tr><th align='left'>Name</th><th align='left'>Size</th></tr>";
  }
  while (foundfile) {
    if (ishtml) {
      returnText += "<tr align='left'><td>" + String(foundfile.name()) + "</td><td>" + humanReadableSize(foundfile.size()) + "</td></tr>";
    } else {
      returnText += "File: " + String(foundfile.name()) + "\n";
    }
    foundfile = root.openNextFile();
  }
  if (ishtml) {
    returnText += "</table>";
  }
  root.close();
  foundfile.close();
  return returnText;
}

// Make size of files human readable
// source: https://github.com/CelliesProjects/minimalUploadAuthESP32
String WebServerStarter::humanReadableSize(const size_t bytes) {
  if (bytes < 1024) return String(bytes) + " B";
  else if (bytes < (1024 * 1024)) return String(bytes / 1024.0) + " KB";
  else if (bytes < (1024 * 1024 * 1024)) return String(bytes / 1024.0 / 1024.0) + " MB";
  else return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
}

// handles uploads
void WebServerStarter::handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
  Serial.println(logmessage);

  if (!index) {
    logmessage = "Upload Start: " + String(filename);
    // open the file on first call and store the file handle in the request object
    request->_tempFile = SPIFFS.open("/" + filename, "w");
    Serial.println(logmessage);
  }

  if (len) {
    // stream the incoming chunk to the opened file
    request->_tempFile.write(data, len);
    logmessage = "Writing file: " + String(filename) + " index=" + String(index) + " len=" + String(len);
    Serial.println(logmessage);
  }

  if (final) {
    logmessage = "Upload Complete: " + String(filename) + ",size: " + String(index + len);
    // close the file handle as the upload is now done
    request->_tempFile.close();
    Serial.println(logmessage);
    request->redirect("/files");
  }
}

String WebServerStarter::processor_upload_file(const String& var) {
	  if (var == "FILELIST") {
    return listFiles(true);
  }
  if (var == "FREESPIFFS") {
    return humanReadableSize((SPIFFS.totalBytes() - SPIFFS.usedBytes()));
  }

  if (var == "USEDSPIFFS") {
    return humanReadableSize(SPIFFS.usedBytes());
  }

  if (var == "TOTALSPIFFS") {
    return humanReadableSize(SPIFFS.totalBytes());
  }

  return String();
}

