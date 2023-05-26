#include "board.h"

void ThreeWayValveBoard::Init(){
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n Hello, I am the main controller of actupuncture.  Commu with I2C , MQTT\n\n");

    // __i2c_master.Init();
}
