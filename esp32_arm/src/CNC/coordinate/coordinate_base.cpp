#include "coordinate_base.h"
#include <HardwareSerial.h>

void FKPosition_XYZRPY::PrintOut(const char* title){
    Serial.println(title);
    Serial.print("    ");
    Serial.print(this->X);
    Serial.print(" , ");
    Serial.print(this->Y);
    Serial.println(" )");
}


void IKPosition_abgdekl::PrintOut(const char* title){
    Serial.print(title);
    Serial.println(" Alpha, Beta, Gamma, Delta..   ");
    for(int i=0; i< 7; i++){
        Serial.print("\t");
        Serial.print(this->Positions[i]);
    }
    Serial.println("");
}
