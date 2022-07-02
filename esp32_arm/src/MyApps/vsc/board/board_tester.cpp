#include "board_tester.h"


void Vsc_BoardTest::LinkBoard(CncBoardBase* board){
    Serial.println("[Info] Vsc_BoardTest::LinkBoard()");
    this->__board = (Vsc_Board*) (board);
    this->LinkCncBoard(board);
}

// Even without angel sensor,  Only a h-bridge driver is there, Should be work properly.
void Vsc_BoardTest::Test_MotorDriver(int loop_count){
    float speed = 123;
    ActuatorDcMotor* motor = (ActuatorDcMotor*)(this->__board->GetActuator(AXIS_ALPHA));
    motor->StartToMove(true, 150);
    delay(5000);
    motor->StartToMove(true, 0);
    delay(5000);
    //.....




    // switch (level){
    //     case 0:
    //         break;
    //     case 1:
    //         // Start to move
    //         // Min speed =0  ; Max Speed = 255;  Don't link angle sensor


    //         // motor->SetTargetPositionTo(false, -899);
    //         break;
    //     // case 2:
    //     //     motor->SetTargetPositionTo(false, 500);
    //     //     motor->SetSpeed(111);
    //     //     motor->StartToMove();
    //     //     break;
    // }
}


/*
# Actuator should support
* start the motor function(CW and CCW ) 
* stop the motor function
* change speed of motor in PWM
* 
*/
void Vsc_BoardTest::Test_Actuator(int loop_count){
    //Speed unit is:    mm/second   or   rad/second
            ActuatorDcMotor* motor = (ActuatorDcMotor*)(this->__board->GetActuator(AXIS_ALPHA));
            motor->SetTargetPositionTo(false, 500);
            motor->SetSpeed(111);   
            for(long i=0; i<88888; i++){
                motor->SpinOnce();
            }
            motor->SetTargetPositionTo(false, -500);
 
    
}

void Vsc_BoardTest::__TestOffset(float value){
    ActuatorDcMotor* motor = (ActuatorDcMotor*) (this->__board->GetActuator(AXIS_ALPHA));
    float xx= 123;
    motor->SetCurrentPositionAs(xx);
    float result = motor->GetCurrentPosition_InCncUnit();
    if (xx == result){
        Serial.println("[Info] Vsc_BoardTest::Test_offset_calculation()  Passed");
    }else{
        Serial.println("[Error] Vsc_BoardTest::Test_offset_calculation() ");
        while(1){
            Serial.print("Err ");
            delay(1000);
        }
    }
}
void Vsc_BoardTest::Test_Offset(int loop_count){
    for (int i=0; i<loop_count; i++){
        Serial.print("[Info]  Vsc_BoardTest::Test_offset_calculation()  Please turn the sensor...");
        delay(3000);
        Serial.print("Stop turning..");
        delay(2000);

        float xx = rand();
        this->__TestOffset(xx);
    }
}

#define INTERVEL 900
void Vsc_BoardTest::Test_AngleSensor(int loop_count){
    MagneticSensorAnalog* sensor = this->__board->GetAngleSensor();
    int down_count = loop_count;
    int interval = 0;

    for(int i=0; i < loop_count; i++){
        sensor->update();
        interval++;
        if (interval > INTERVEL){
            Serial.print(down_count);
            Serial.print("\t");
            Serial.print(sensor->getAngle());
            Serial.print("\t");
            Serial.print(sensor->getFullRotations());
            Serial.print("\t");
            Serial.println(sensor->getVelocity());
            down_count--;
            interval = 0;
        }
        // delay(500); 
    }
    Serial.println("[Info] Vsc_BoardTest::Test_AngleSensor() is done.");

}

void Vsc_BoardTest::Test_AllHomers(int loop_count){
    SingleAxisHomer* homer = this->__board->GetHomer(AXIS_ALPHA);
    for (int i=0; i<loop_count; i++){
        Serial.print(homer->IsTriged());
        Serial.println("Homer is Triger");
        delay(1000);
    }
}
