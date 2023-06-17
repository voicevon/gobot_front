#include "feng.hpp"
#include "yin.hpp"
#include "Arduino.h"

void setup(){
	setup_feng();
	Serial.println("Start to run Yin setup()");
	
	setup_yin();

}

void loop(){
	loop_feng();
	// loop_yin();
}