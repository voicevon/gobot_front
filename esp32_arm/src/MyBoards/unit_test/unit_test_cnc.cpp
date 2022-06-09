#include "unit_test_cnc.h"

void BoardTestCnc::Test_AllHomers(int loop_count){
// void BoardTestCnc::Test_AllHomers(int loop_count, char* axis_list, int axis_count){
    if (loop_count==0) return;

    Serial.println("[Info] BoardTestCnc::Test_AllHomers()");

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

void BoardTestCnc::Test_Stepper(int loop_count, char axis_name, int distance_in_step, StepControl* control){
    if (loop_count==0) return;
    Serial.print("[Info] BoardTestCnc::Test_Stepper()  axis_name= ");
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
