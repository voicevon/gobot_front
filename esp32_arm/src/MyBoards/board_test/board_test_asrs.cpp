#include "board_test_asrs.h"

void UnitTestAsrs::Test_Sharp_IrSensor(int loop_count){
    Serial.println("[Info] BoardAllInOne::Test_Sharp_IrSensor() is entering.");
    JettySensor_SharpIrOnAds1115* sharp = this->__board->GetJettySensor();
    for (int i =0; i<loop_count; i++){
        float a = sharp->ReadDistance(JettySensorBase::IR_POSITION::LOWER_INNER);
        float b = sharp->ReadDistance(JettySensorBase::IR_POSITION::LOWER_OUTER);
        float c = sharp->ReadDistance(JettySensorBase::IR_POSITION::UPPER_INNER);
        float f = sharp->ReadDistance(JettySensorBase::IR_POSITION::UPPER_OUTER);
        Serial.print(a);
        Serial.print("\t");
        Serial.print(b);
        Serial.print("\t");
        Serial.print(c);
        Serial.print("\t");
        Serial.println(f);
        delay(300);
    }
}