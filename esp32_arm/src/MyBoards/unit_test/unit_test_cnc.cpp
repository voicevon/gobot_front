#include "unit_test_cnc.h"

void UnitTestCnc::Test_AllHomers(int loop_count){
    SingleAxisHomer* z = this->__board->GetHomer('Z');
    SingleAxisHomer* y = this->__board->GetHomer('Y');
    bool zh,yh;
    for(int i=0; i<loop_count; i++){
        zh= z->IsTriged();
        yh = y->IsTriged();
        Serial.print("z: ");
        Serial.print(zh);
        Serial.print("\ty: 444");
        Serial.println(yh);
        delay(200);
    }
}

void UnitTestCnc::Test_Stepper(int loop_count, char axis_name, int distance_in_step, StepControl* control){
    Stepper* ss = this->__board->GetStepper(axis_name);
    this->__board->EnableMotor(axis_name, true);
    ss->setAcceleration(distance_in_step / 2);
    for(int i=0; i<loop_count; i++){
        //CW
        ss->setTargetAbs(distance_in_step);
        control->move(*ss);
        delay(500);
        //CCW
        ss->setTargetAbs(0);
        control->move(*ss);
        delay(500);
    }
}
