
extern "C"{
	extern void __main();
}

#include <Arduino.h>

void setup(){
	// Serial.begin(115200);
	// Serial.print("Leaving Arduino");
	__main();
}

void loop(){
	while(true){
		delay(100);
		// Serial.print(".");
	}
}


