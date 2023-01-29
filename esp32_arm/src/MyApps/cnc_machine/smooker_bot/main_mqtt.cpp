//https://www.electronicshub.org/wp-content/uploads/2021/02/ESP32-Pinout-1.jpg

#include "all_applications.h"
#ifdef I_AM_SMOKE_MACHINE_MQTT

#include "MyLibs/MyFunctions.hpp"

#include <HardwareSerial.h>
#include "smoke_bot.h"
#include "CNC/command_queue.h"
#include "webCommu.h"

// std::string topic = "actu/dev001/yongquan"; 
// std::string payload ="OFF";

SmokeBot* mybot;
CommandQueue myCommandQueue = CommandQueue();
// int distance = 100;
// int pause_second = 20;
// Gcode gcode("");
// std::string strGcode;
// String strGcode;

// void app_mqtt_subscribe(){
//   mqttClient.subscribe("smokebot/distance", 2);
//   mqttClient.subscribe("smokebot/pause_second", 2);
//   Serial.println("Subscribed smokebot/*");
// }

// void app_mqtt_received_message( char* topic, char* payload){
//   const char * cc = (const char*)(payload);

//   if(strcmp(topic, "smokebot/distance") == 0) {   // char len = 17
//     distance = atof(cc);
//   }
//   else if(strcmp(topic, "smokebot/pause_second")==0){    // char len = 21
//     pause_second = atof(cc);
//   }else{
//     Serial.print(" app_mqtt_received_message() Warning   ");
//     Serial.print (topic);
//     Serial.print("   ");
//     Serial.print (payload);
//   }

// }

int get_int(){
  return 123;
}
void test_string(){
  int x= 123;
  String strG4 = "G4S";
  String sg = strG4 + x;
  Serial.println(sg);
  while (true){

  } 
}
void setup() {
	Serial.begin(115200);
	Serial.println("Hi there, I am smoke robot, Have a good day");
	// test_string();


	setup_webcommu();
	// setup_wifi_mqtt();
	mybot = new SmokeBot();
	myCommandQueue.LinkRobot(mybot);
	mybot->Init_Gpio();
	mybot->Init_Linkage();
	Serial.println("\nSet up is done .....");
	String strG28 = "G28";
	bool result = myCommandQueue.AppendGcodeCommand(strG28);
	myCommandQueue.SpinOnce();
	
	// while (!mqttClient.connected())
	//     delay(100);
	// mybot->HomeAllAxises();
}


void loop() {
	String strG4 = "G4S";
	String strG1 = "G1X";
	WebCommu_SpinOnce();
	mybot->SpinOnce();
	myCommandQueue.SpinOnce();
	if (!varOnOff) return;

	if ((mybot->State == RobotBase::IDLE) && (myCommandQueue.BufferIsEmpty())){
		int distance = float(var_per_volume) * 90.0;
		String sg = strG1 + distance;
		bool result = myCommandQueue.AppendGcodeCommand(sg);

		sg = strG4 + var_sleep_in_second;
		result = myCommandQueue.AppendGcodeCommand(sg);

		sg = strG1 + 0.1;
		result = myCommandQueue.AppendGcodeCommand(sg);
		var_done_count++;
	}

}
#endif