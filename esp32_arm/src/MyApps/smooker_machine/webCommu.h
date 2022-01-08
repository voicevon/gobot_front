//==================================
//ESP32 WebSocket Server: Toggle LED
//by: Ulas Dikme
// https://akuzechie.blogspot.com/2020/12/esp32-websocket-server.html
//==================================
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
//-----------------------------------------------
const char* ssid = "CentOS";
const char* password = "1234567890";
//-----------------------------------------------
#define LED 2
//-----------------------------------------------
WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
//-----------------------------------------------
String JSONtxt;
boolean varOnOff = false; 
boolean varReset = false;
int var_done_count = 0;
int var_total_count = 300;
int var_per_volume = 20;
int var_pull_in_second = 3;
int var_sleep_in_second = 5;
//-----------------------------------------------
#include "html_page.h"
#include "web_functions.h"
//====================================================================
void setup_webcommu()
{
  Serial.begin(115200); pinMode(LED, OUTPUT);
  //-----------------------------------------------
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){Serial.print("."); delay(500);}
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  //-----------------------------------------------
  server.on("/", webpage);
  //-----------------------------------------------
  server.begin(); webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}



void send_to_client(){
  String strOnOff = "OFF";
  if(varOnOff) strOnOff = "ON";
  JSONtxt = "{\"varOnOff\":\""+strOnOff+"\","
            + "\"var_done_count\"" +":\"" + var_done_count + "\","
            + "\"var_total_count\"" +":\"" + var_total_count + "\","
            + "\"var_per_volume\"" +":\"" + var_per_volume + "\","            
            + "\"var_pull_in_second\"" +":\"" + var_pull_in_second + "\","
            + "\"var_sleep_in_second\"" +":\"" + var_sleep_in_second + "\""
              +"}";
  // Serial.print("------------------------------------------\n");
  // Serial.print(JSONtxt);
  // Serial.print("------------------------------------------\n");
  webSocket.broadcastTXT(JSONtxt);
}
//====================================================================
void WebCommu_SpinOnce()
{
  webSocket.loop(); 
  server.handleClient();
  //-----------------------------------------------
  if(varOnOff == false) 
    digitalWrite(LED, LOW);
  else 
    digitalWrite(LED, HIGH);
  //-----------------------------------------------
  if (true){
    // Some var  updated, need to tell the client to sync those data.
    send_to_client();
  }
}
