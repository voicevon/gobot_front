#include "board_tester.h"


void Vsc_BoardTest::LinkBoard(CncBoardBase* board){
    Serial.println("[Info] Vsc_BoardTest::LinkBoard()");
    this->__board = (Vsc_Board*) (board);
    this->LinkCncBoard(board);
}


void Vsc_BoardTest::Test_Motor(){
    // Test motor CW and CCW speed 
    /*
    #  Motor should support
    * start the motor function(CW and CCW ) 
    * stop the motor function
    * change speed of motor in PWM
    * 
     */

    ActuatorDcMotor* motor = this->__board->GetDcMotor();
    motor->SetTargetPositionTo(true, 500);
    motor->SetSpeed(111);
    motor->StartToMove();
    // motor->SetTargetPositionTo(false, -899);
}

void Vsc_BoardTest::__TestOffset(float value){
    ActuatorDcMotor* motor = this->__board->GetDcMotor();
    float xx= 123;
    motor->SetCurrentPositionAs(xx);
    float result = motor->GetCurrentPosition_InCncUnit();
    if (xx == result){
        Serial.println("[Info] Vsc_BoardTest::Test_offset_calculation()  Passed");
    }else{
        Serial.println("[Error] Vsc_BoardTest::Test_offset_calculation() ");
        while(1){
            Serial.print('Err ');
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


void Vsc_BoardTest::Test_AngleSensor(int loop_count){
    MagneticSensorAnalog* sensor = this->__board->GetSensor();

    for(int i=0; i < loop_count; i++){
        sensor->update();
        Serial.print(sensor->getAngle());
        Serial.print("\t");
        Serial.print(sensor->getFullRotations());
        Serial.print("\t");
        Serial.println(sensor->getVelocity());
        delay(500); 
    }

}