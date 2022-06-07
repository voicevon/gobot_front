#include "unit_test_agv.h"

void UnitTestAgv::Test_TrackSensor(int loop_count){
    TrackSensorBase* sensor = this->__board->GetTrackSensor();
    for (int i=0; i<loop_count; i++){
        int16_t error = sensor->ReadAlignmentError();
        Serial.println(error);
        delay(200);
    }
}

void UnitTestAgv::Test_ObstacleSensor(int loop_count){
    ObstacleSensorBase* sensor = this->__board->GetObstacleSensor();
    for(int i=0; i<loop_count; i++){
        bool blocked = sensor->DetectObstacle();
        Serial.println(blocked);
        delay(200);
    }
}

