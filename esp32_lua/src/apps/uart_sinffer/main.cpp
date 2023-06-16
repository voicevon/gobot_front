
extern "C"{
	extern void __main();
}

#include <Arduino.h>
#include "von/cpp/wifi/task_wifi.h"
#include "lua_driver/task_mqtt.h"
#include "von/cpp/utility/logger.h"

TaskHandle_t task_Mqtt;

void setup(){
	Serial.begin(115200);
	__main();

	bool callable_by_c = true;
	xTaskCreate(TaskMqtt, "Mqtt", 10000, (void*) (&callable_by_c),  1, &task_Mqtt);   
	
	ConnectToWifi_FakeTask();

	Logger::Info("Arduino setup() is done..");
}

void loop(){
	while(true){
		delay(100);
		// Serial.print(".");
	}
}


