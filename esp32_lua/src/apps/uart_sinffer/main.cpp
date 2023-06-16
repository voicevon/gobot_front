
extern "C"{
	extern void __main();
}

#include <Arduino.h>
#include "von/cpp/wifi/task_wifi.h"
#include "von/cpp/mqtt/task_mqtt.h"

#include "von/cpp/utility/logger.h"
#include "lua_driver/api_common.hpp"
#include "von/cpp/mqtt/subscriber/mqtt_subscriber_manager.h"
#include "von/cpp/mqtt/g_var.h"
#include "von/cpp/mqtt/subscriber/mqtt_subscriber_string.h"


MqttSubscriber_String subsribers[8];

void InitSubscribers(){
	// subsribers[0].SubscribeMqtt("abc");
	// subsribers[1].SubscribeMqtt("abc");
	// subsribers[2].SubscribeMqtt("abc");
	// subsribers[3].SubscribeMqtt("abc");
	// subsribers[4].SubscribeMqtt("abc");
	// subsribers[5].SubscribeMqtt("abc");
	// subsribers[6].SubscribeMqtt("abc");
	// subsribers[7].SubscribeMqtt("abc");
	for(int i=0; i<8; i++){
		gs_MqttSubscriberManager::Instance().AddSubscriber("abc", &subsribers[i]);
	}
}

void mqtt_publish(const char* topic, const char* payload){
    g_mqttClient.publish(topic, 2,true, payload);
}

void setup_callback(){
	set_callback_mqtt_publish(mqtt_publish);
	set_callback_mqtt_subscribe(gs_MqttSubscriberManager::Instance().mqtt_subscribe_with_topicIndex);
	// set_callback_read_first_topic(mqtt_read_first_topic);
	set_callback_mqtt_read_payload(gs_MqttSubscriberManager::Instance().mqtt_read_payload);
	set_callback_mqtt_release_buffer(gs_MqttSubscriberManager::Instance().mqtt_release_buffer);
}



void setup(){
	Serial.begin(115200);
	__main();
	xTaskCreate(TaskMqtt, "Mqtt", 10000, NULL,  1, &task_Mqtt);   
	ConnectToWifi_FakeTask();
	setup_callback();
	InitSubscribers();

	Logger::Info("Arduino setup() is done..");
}

void loop(){
	while(true){
		delay(100);
		// Serial.print(".");
	}
}


