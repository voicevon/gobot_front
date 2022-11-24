#include "coordinate_base.h"
#include <HardwareSerial.h>

void FKPosition_XYZRPY::PrintOut(const char* title){
    Serial.print(title);
    Serial.print("    ");
    Serial.print(this->X);
    Serial.print(" , ");
    Serial.print(this->Y);
    Serial.println(" )");
}
