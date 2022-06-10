#include "board_test_agv.h"

void UnitTestAgv::Test_TrackSensor(int loop_count){
    if (loop_count == 0) return;

    Serial.println("[Info] UnitTestAgv::Test_TrackSensor()");
    delay(1000);
    TrackSensorBase* sensor = this->__board->GetTrackSensor();
    for (int i=0; i<loop_count; i++){
        int16_t error = sensor->ReadAlignmentError();
        Serial.println(error);
        delay(200);
    }
}

void UnitTestAgv::Test_ObstacleSensor(int loop_count){
    if (loop_count == 0) return;

    Serial.println("[Info] UnitTestAgv::Test_ObstacleSensor()");
    delay(1000);
    ObstacleSensorBase* sensor = this->__board->GetObstacleSensor();
    for(int i=0; i<loop_count; i++){
        bool blocked = sensor->DetectObstacle();
        Serial.println(blocked);
        delay(200);
    }
}


void UnitTestAgv::Test_TrackLight(int loop_count){
    if (loop_count == 0) return;

    Serial.println("[Info] UnitTestAgv::Test_TrackLight()");
    delay(3000);
    LightBase* light = this->__board->GetTrackLight();
    for(int i=0; i<loop_count; i++){
        Serial.print(" On ");
        light->TurnOn(true);
        delay(1000);
        Serial.println(" Off ");
        light->TurnOn(false);
        delay(1000);
    } 
}

void UnitTestAgv::Test_RfidReader(int loop_count){
    if (loop_count == 0) return;

    Serial.println("[Info] UnitTestAgv::Test_RfidReader()");
    delay(3000);
    MapSiteReaderBase* reader = this->__board->GetMapSiteReader();
    for(int i=0; i<loop_count; i++){
        int id = reader->ReadMapSiteId();
        Serial.println(id);
        delay(100);
    }  
}

void UnitTestAgv::Test_Mover(int loop_count){
    if (loop_count == 0) return;

    Serial.println("[Info] UnitTestAgv::Test_Mover()");
    MoverBase* mover = this->__board->GetMover();
    // mover->SetForwdingSpeed(100, 0);
    // delay(50000);

    for(int i=0; i<loop_count; i++){
        Serial.print("Forward    ");
        for(int speed=-100; speed<100; speed+=10){
            mover->SetForwdingSpeed(speed, 0);
            delay(500);
        }
        Serial.println("Backward   ");
        for(int speed=100; speed>-100; speed-=10){
            mover->SetForwdingSpeed(speed, 0);
            delay(500);
        }
    } 
}

void UnitTestAgv::Test_Battery(int loop_count){
    if (loop_count == 0) return;

    Serial.println("[Info] UnitTestAgv::Test_Battery()");
    delay(1000);
    float volt = 0.0;
    for(int i=0; i<loop_count; i++){
        volt = this->__board->ReadBatteryVolt();
        // bool blocked = sensor->DetectObstacle();
        Serial.println(volt);
        delay(1000);
    }
}

