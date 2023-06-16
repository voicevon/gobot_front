
extern "C"{
	extern void __main();
}

#include <Arduino.h>
#include "von/c/task/wifi/task_wifi.h"
#include "lua_driver/task_mqtt.h"

TaskHandle_t* task_WiFi;
TaskHandle_t* task_Mqtt;

void setup(){
	__main();
	
	xTaskCreate(TaskWiFi, "WiFi", 10000, NULL,  1, task_WiFi);   
	xTaskCreate(TaskMqtt, "Mqtt", 10000, NULL,  1, task_Mqtt);   

}

void loop(){
	while(true){
		delay(100);
		// Serial.print(".");
	}
}


