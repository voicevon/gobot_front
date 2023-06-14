
extern "C"{
	extern void __main();
}

#include <Arduino.h>

void setup(){
	__main();
}

void loop(){
	while(true){
		delay(100);
	}
}


