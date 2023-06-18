#include "von/utility/wifi/task_wifi.h"
#include "von/utility/mqtt/g_var.h"
#include "von/utility/mqtt/task_mqtt.h"

#include "von/utility/logger.h"

#include "lua_driver/api_rtos.h"

#include "driver_setup.h"
#include "Arduino.h"

extern "C" { extern void InitRtosTasks();}
extern void setup_yin();

void setup(){
	WiFiTask_config wifi_task_config;
	wifi_task_config.ssid = "FuckGFW";
	wifi_task_config.password = "refuckgfw";
	wifi_task_config.ControlMqttTask = false;
	wifi_task_config.Asyncconnection = false;

	Serial.begin(115200);
	// InitRtosTasks();
	// xTaskCreate(TaskMqtt, "Mqtt", 10000, NULL,  1, &task_Mqtt);   
	
	ConnectToWifi_FakeTask(&wifi_task_config);
	// setup_feng();
	// while (!g_mqttClient.connected()){
	// 	vTaskDelay(1);
	// }
	// setup_callback();
	// InitSubscribers();

	Logger::Info("Arduino setup() is done..");
	Serial.println("Start to run Yin setup()");
	
	setup_yin();

}

void loop(){
	// for(int i=0; i<4; i++){
	// 	leds[i].SpinOnce();
	// }
	vTaskDelay(1000);
}