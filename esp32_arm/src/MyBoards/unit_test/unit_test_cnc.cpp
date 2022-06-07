#include "unit_test_cnc.h"

void UnitTestCnc::Test_AllHomers(int loop_count){
    Serial.println("[Info] UnitTestCnc::Test_AllHomers()");
    delay(3000);

    SingleAxisHomer* hz = this->__board->GetHomer('Z');
    SingleAxisHomer* hy = this->__board->GetHomer('Y');
    bool zt,yt;
    for(int i=0; i<loop_count; i++){
        zt= hz->IsTriged();
        yt = hy->IsTriged();
        Serial.print("z: ");
        Serial.print(zt);
        Serial.print("\ty: ");
        Serial.println(yt);
        delay(200);
    }
}

void UnitTestCnc::Test_Stepper(int loop_count, char axis_name, int distance_in_step, StepControl* control){
    Serial.print("[Info] UnitTestCnc::Test_Stepper()  axis_name= ");
    Serial.println(axis_name);
    delay(3000);

    Stepper* ss = this->__board->GetStepper(axis_name);
    this->__board->EnableMotor(axis_name, true);
    ss->setAcceleration(distance_in_step / 2);
    for(int i=0; i<loop_count; i++){
        //CW
        Serial.println("Spin CW.");
        ss->setTargetAbs(distance_in_step);
        control->move(*ss);
        delay(500);
        //CCW
        Serial.println("Spin CCW.");
        ss->setTargetAbs(0);
        control->move(*ss);
        delay(500);
    }
    this->__board->EnableMotor(axis_name, false);
}
