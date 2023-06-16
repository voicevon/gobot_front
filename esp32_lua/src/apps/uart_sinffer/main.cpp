
extern "C"{
	extern void __main();
}

#include <Arduino.h>
#include "von/cpp/wifi/task_wifi.h"
#include "lua_driver/task_mqtt.h"
#include "von/cpp/utility/logger.h"
#include "lua_driver/api_common.hpp"

TaskHandle_t task_Mqtt;

void setup_callback(){
	set_callback_mqtt_publish(SmartMqttClient::Instance().mqtt_publish);
	set_callback_mqtt_subscribe(SmartMqttClient::Instance().mqtt_subscribe);
	// set_callback_read_first_topic(mqtt_read_first_topic);
	set_callback_mqtt_read_payload(SmartMqttClient::Instance().mqtt_read_payload);
	set_callback_mqtt_release_buffer(SmartMqttClient::Instance().mqtt_release_buffer);
}

void setup(){
	Serial.begin(115200);
	__main();
	xTaskCreate(TaskMqtt, "Mqtt", 10000, NULL,  1, &task_Mqtt);   
	ConnectToWifi_FakeTask();
	setup_callback();

	Logger::Info("Arduino setup() is done..");
}

void loop(){
	while(true){
		delay(100);
		// Serial.print(".");
	}
}


