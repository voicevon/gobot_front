


#include <Arduino.h>
#include "von/utility/wifi/task_wifi.hpp"


#include "von/utility/mqtt/g_var.h"
#include "von/utility/mqtt/task_mqtt.h"
#include "von/utility/mqtt/subscriber/mqtt_subscriber_manager.h"
#include "von/component/display/mono_led_gpio.h"
#include "von/utility/logger.h"

#include "lua_driver/api_mqtt.hpp"
#include "lua_driver/api_leds.hpp"
#include "lua_driver/global_const.hpp"



MqttSubscriberBase subsribers[8];
Mono_Led_GPIO leds[4];
extern "C"{	extern void __main(); }
#define PIN_LED_POWER 22
#define PIN_LED_UART_MASTER_RX_TX 23
#define PIN_LED_UART_SLAVE_RX_TX 24
#define PIN_LED_MQTT_RX_TX 25

void OnMqttReceived(MqttSubscriberBase* subscriber){
	Logger::Info("OnMqttReceived()");
	Logger::Print(String(subscriber->GetId()).c_str(), subscriber->GetMqttTopic()->c_str() );
	Logger::Print(String(subscriber ->GetPayloadLength()).c_str(), subscriber->ReadPayload_as_string());
	// set EventBit
	xEventGroupSetBits(my_EventGroup,  1 << subscriber->GetId());  // set eventbit	
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
		subsribers[i].SetCallback_onGot_MqttMessage(OnMqttReceived);
	}
}

void mqtt_publish(const char* topic, const char* payload){
    g_mqttClient.publish(topic, 2,true, payload);
}

void InitLeds(){
	leds[0].Init(0, PIN_LED_POWER, HIGH);
	leds[1].Init(1, PIN_LED_UART_MASTER_RX_TX, HIGH);
	leds[2].Init(2, PIN_LED_UART_SLAVE_RX_TX, HIGH);
	leds[3].Init(3, PIN_LED_MQTT_RX_TX, HIGH);
	
}
void SetLedState(int led_index, int state){
	leds[led_index].SetState(static_cast<Mono_Led_GPIO::EnumState>(state));
}

void setup_callback(){
	set_callback_mqtt_publish(mqtt_publish);

	set_callback_mqtt_subscribe(gs_MqttSubscriberManager::Instance().mqtt_subscribe_with_topicIndex);
	// set_callback_read_first_topic(mqtt_read_first_topic);
	set_callback_mqtt_read_payload(gs_MqttSubscriberManager::Instance().mqtt_read_payload);
	set_callback_mqtt_release_buffer(gs_MqttSubscriberManager::Instance().mqtt_release_buffer);
	InitLeds();
	InitCallback_CppSetLedState(SetLedState);
}


void setup_feng(){
	WiFiCredential wifi_credential;
	wifi_credential.ssid = "Perfect";
	wifi_credential.password = "1234567890";

	Serial.begin(115200);
	__main();
	xTaskCreate(TaskMqtt, "Mqtt", 10000, NULL,  1, &task_Mqtt);   
	
	ConnectToWifi_FakeTask(&wifi_credential);
	setup_callback();
	while (!g_mqttClient.connected()){
		vTaskDelay(1);
	}
	InitSubscribers();

	Logger::Info("Arduino setup() is done..");
}

void loop_feng(){
	while(true){
		for(int i=0; i<4; i++){
			leds[i].SpinOnce();
		}
	}
}


