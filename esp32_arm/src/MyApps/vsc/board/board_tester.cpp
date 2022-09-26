#include "board_tester.h"


// void Vsc_BoardTest::LinkBoard(CncBoardBase* board){
//     Serial.println("[Info] Vsc_BoardTest::LinkBoard()");
//     this->__board = (Vsc_Board*) (board);
//     this->LinkCncBoard(board);
// }

// Even without angel sensor,  Only a h-bridge driver is there, Should be work properly.

void Vsc_BoardTest::Test_MotorDriver(ActuatorDcMotor* motor,int loop_count){
    uint32_t speed = 255;
    uint32_t delay_ms = 10000;
    //  = this->__board->GetActuator(AXIS_ALPHA);
    for(int i=0; i<loop_count ;i++){    
        Serial.print("motor test start");
        Serial.println("       motor CW ");
        motor->Test_PwmSpeed(true, speed);
        delay(delay_ms);
        
        Serial.print("       motor stop");
        motor->Test_PwmSpeed(true, 0);
        delay(delay_ms);
        
        Serial.print("       motor CCW");
        motor->Test_PwmSpeed(false, speed);
        delay(delay_ms);

        Serial.print("       motor stop");
        motor->Test_PwmSpeed(false, 0);
        delay(delay_ms);
        Serial.println("     motor test end");
    }
}


/*
# Actuator should support below requirements:
*    start the motor function(CW and CCW ) 
*    stop the motor function
*    change speed of motor in PWM
* 
*/
void Vsc_BoardTest::Test_Actuator(ActuatorDcMotor* motor, int loop_count){
    //Speed unit is:    mm/second   or   rad/second
    float distance;
    // ActuatorDcMotor* motor = this->__board->GetActuator(AXIS_ALPHA);   // todo: 
    float speed = 12;
    LineSegment move;
    move.IsAbsTargetPosition = false;
    move.TargetPosition = 500;
    for (int i =0; i<loop_count; i++){
        // motor->SetTargetPositionTo(false, 500);
 
        motor->UpdateMovement(&move);
        motor->SetSpeed(speed);   
        distance = motor->GetAbsDistanceToTarget_InCncUnit();
        while (distance > 10){
            motor->SpinOnce();
            distance = motor->GetAbsDistanceToTarget_InCncUnit();
        }
        delay(5000);


        // motor->SetTargetPositionTo(false, 0);
        move.TargetPosition = 0;
        motor->UpdateMovement(&move);

        motor->SetSpeed(speed/2);   
        distance = motor->GetAbsDistanceToTarget_InCncUnit();
        while (distance > 10){
            motor->SpinOnce();
            distance = motor->GetAbsDistanceToTarget_InCncUnit();
        }
        delay(5000);
    }
 
    
}

void Vsc_BoardTest::__TestOffset(float value){
    // ActuatorDcMotor* motor = (ActuatorDcMotor*) (this->__board->GetActuator(AXIS_ALPHA));
    ActuatorDcMotor* motor ;  // todo
    float xx= 123;
    motor->SetCurrentPositionAs(xx);
    float result = motor->GetCurrentPosition();
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
void Vsc_BoardTest::Test_AngleSensor(MagneticSensorAnalog* sensor, int loop_count){
    if (loop_count==0) return;

    //  = this->__board->GetAngleSensor();
    int down_count = loop_count;   // For display how many count is left  only. No other purpose.
    int interval = 0;
    LowPassFilter filter = LowPassFilter(0.05);
    sensor->init();
    for(int i=0; i < loop_count; i++){
        while (interval < INTERVEL){
            sensor->update();
            float filtered_speed = filter(sensor->getVelocity());
            interval++;
            if (interval == INTERVEL){
                Serial.print(down_count);
                Serial.print("\t");
                Serial.print(sensor->getAngle()* RAD_TO_DEG);
                Serial.print("\t");
                Serial.print(sensor->getFullRotations());
                Serial.print("\t");
                Serial.println(filtered_speed * RAD_TO_DEG);
                down_count--;
                interval = 0;
            }
        }
    }

    Serial.println("[Info] Vsc_BoardTest::Test_AngleSensor() is done.");

}


void Vsc_BoardTest::Test_EncoderSensor(Encoder* sensor, int loop_count){
    if (loop_count==0) return;

    //  = this->__board->GetAngleSensor();
    int down_count = loop_count;   // For display how many count is left  only. No other purpose.
    int interval = 0;
    // LowPassFilter filter = LowPassFilter(0.05);
    // sensor->init();
    for(int i=0; i < loop_count; i++){
        while (interval < INTERVEL){
            sensor->update();
            // float filtered_speed = filter(sensor->getVelocity());
            interval++;
            if (interval == INTERVEL){
                Serial.print(down_count);
                Serial.print("\t");
                Serial.print(sensor->getAngle()* RAD_TO_DEG);
                Serial.print("\t");
                Serial.print(sensor->getFullRotations());
                Serial.print("\t");
                Serial.println(sensor->getVelocity() * RAD_TO_DEG);
                down_count--;
                interval = 0;
            }
        }
    }

    Serial.println("[Info] Vsc_BoardTest::Test_AngleSensor() is done.");

}
