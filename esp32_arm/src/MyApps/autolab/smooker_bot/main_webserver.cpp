//https://www.electronicshub.org/wp-content/uploads/2021/02/ESP32-Pinout-1.jpg



#include "MyLibs/MyFunctions.hpp"
#include "webCommu.h"
#include "board/smoker_board.h"
#include "CNC/gcode/gcode_queue.h"
#include "robot/smoker_robot.h"

#include "../select_app.h"
#ifdef I_AM_SMOKE_BOT

Smoker_Board board;
SmokerRobot cnc;

GcodeQueue myCommandQueue = GcodeQueue();
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



void setup() {
	board.Init();
	cnc.Init(&board);

	setup_webcommu();
	cnc.LinkLocalGcodeQueue_AsConsumer(&myCommandQueue);
	
	Serial.println("\nSet up is done .....");
	String strG28 = "G28";
	myCommandQueue.AppendGcodeCommand(strG28);
	// myCommandQueue.SpinOnce();
	// mybot.SpinOnce();
	
	// while (!mqttClient.connected())
	//     delay(100);
	// mybot->HomeAllAxises();
}
int get_gcode_distance(float distance_in_ml){
	int result = float(distance_in_ml) * 538.9;
	return result;
} 
void loop() {
	String strG4 = "G4S";
	String strG1 = "G1X";
	WebCommu_SpinOnce();
	cnc.SpinOnce();
	// myCommandQueue.SpinOnce();
	if (!varOnOff) return;
	if (var_done_count >= var_total_count) return;
	// if ((cnc.State == CncState::IDLE) && (myCommandQueue.BufferIsEmpty())){
	// if ((cnc.GetMoverState == CncState::IDLE) && (myCommandQueue.BufferIsEmpty())){
	if ((myCommandQueue.BufferIsEmpty())){   //TODO: above

		// Go back to home position, keep a 0.1mm position
		int speed = float(var_per_volume) / float(var_pull_in_second) * 540;
		if (speed < 100) speed=100;
		if (speed > 400000) speed=400000;  
		String sg3 = strG1 + get_gcode_distance(53.0f - var_per_volume);
		sg3.concat("F");
		sg3.concat(speed);
		myCommandQueue.AppendGcodeCommand(sg3);

		// Push to the end position, might stop here.
		// int distance = get_gcode_distance(float(var_per_volume));
		String sg = strG1;
		sg.concat(get_gcode_distance(53));
		myCommandQueue.AppendGcodeCommand(sg);

		// sleep a while 
		String sg2 = strG4 + var_sleep_in_second;
		myCommandQueue.AppendGcodeCommand(sg2);

		var_done_count++;


	}

}
#endif