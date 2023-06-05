#include "board.h"

void AcupunctureBoard_2023::Init(const char* app_welcome_statement){
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n Hello, I am the main controller of actupuncture.  Commu with I2C , MQTT\n\n");

    __i2c_master.Init();
}
