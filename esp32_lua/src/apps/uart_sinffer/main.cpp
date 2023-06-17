
extern "C"{
	extern void __main();
}

#include <Arduino.h>
#include "von/cpp/wifi/task_wifi.h"
#include "von/cpp/mqtt/task_mqtt.h"
#include "von/cpp/mqtt/subscriber/mqtt_subscriber_manager.h"
#include "von/cpp/mqtt/g_var.h"
// #include "von/cpp/mqtt/subscriber/mqtt_subscriber_base.h"
#include "von/cpp/utility/logger.h"
#include "lua_driver/api_common.hpp"


MqttSubscriberBase subsribers[8];


void ReceiveMqtt(MqttSubscriberBase* subscriber){
	Logger::Info("ReceiveMqtt()");
	Logger::Print(subscriber->GetMqttTopic()->c_str(), subscriber->ReadPayload_as_string());
}

void InitSubscribers(){
	gs_MqttSubscriberManager::Instance().Init(false);
	char* topic_0 = "server/time/now";
	char* topic_1 = "server/time/now/string";
	char* topic_2 = "server/time/now/string";
	char* topic_3 = "server/time/now/string";
	char* topic_4 = "server/time/now/string";
	char* topic_5 = "server/time/now/string";
	char* topic_6 = "server/time/now/string";
	char* topic_7 = "server/time/now/string";

	char* topics[8];
	topics[0] = topic_0;
	topics[1] = topic_1;
	topics[2] = topic_2;
	topics[3] = topic_3;
	topics[4] = topic_4;
	topics[5] = topic_5;
	topics[6] = topic_6;
	topics[7] = topic_7;

	
	for(int i=0; i<8; i++){
		gs_MqttSubscriberManager::Instance().AddSubscriber(topics[i], &subsribers[i]);
		subsribers[i].SetCallback_onGot_MqttMessage(ReceiveMqtt);
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
	while (!g_mqttClient.connected()){
		vTaskDelay(1);
	}
	InitSubscribers();

	Logger::Info("Arduino setup() is done..");
}

void loop(){
	while(true){
		delay(100);
		// Serial.print(".");
	}
}


