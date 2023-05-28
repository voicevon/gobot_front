#include "board.h"

void ThreeWayValveBoard::Init(){
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n Hello, I am the main controller of actupuncture.  Commu with I2C , MQTT\n\n");


    // __dc_motor.LinkHBridgeDriver(123);
    __reciprocator.LinkActuator(&__dc_motor);
}

void ThreeWayValveBoard::SetMode_GoStraight();
void ThreeWayValveBoard::SetMode_TurnRight();