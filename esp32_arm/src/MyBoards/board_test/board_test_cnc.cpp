#include "board_test_cnc.h"

// void BoardTestCnc::Test_SingleHomer(int loop_count,char axis_name){
// // void BoardTestCnc::Test_AllHomers(int loop_count, char* axis_list, int axis_count){
//     if (loop_count==0) return;

//     Serial.println("[Warn] BoardTestCnc::Test_SingleHomer()  This is a TODO task.");


// }

// void BoardTestCnc::Test_Stepper(int loop_count, char axis_name, int distance_in_step, StepControl* control){
//     if (loop_count==0) return;
//     Serial.print("[Info] BoardTestCnc::Test_Stepper()  axis_name= ");
//     Serial.println(axis_name);
//     delay(3000);

//     Stepper* ss = this->__board->GetStepper(axis_name);
//     this->__board->EnableMotor(axis_name, true);
//     ss->setAcceleration(distance_in_step / 2);
//     for(int i=0; i<loop_count; i++){
//         //CW
//         Serial.println("Spin CW.");
//         ss->setTargetAbs(distance_in_step);
//         control->move(*ss);
//         delay(500);
//         //CCW
//         Serial.println("Spin CCW.");
//         ss->setTargetAbs(0);
//         control->move(*ss);
//         delay(500);
//     }
//     this->__board->EnableMotor(axis_name, false);
// }


// void BoardTestCnc::Test_StepperEnablePin(int loop_count, char axis_name){
//     if (loop_count==0) return;
//     Serial.print("[Info] BoardTestCnc::Test_StepperEnablePin()  axis_name= ");
//     Serial.println(axis_name);

//     for(int i=0; i<loop_count; i++){
//         this->__board->EnableMotor(axis_name, true);
//         // Serial.println("E.");
//         delay(3000);
//         this->__board->EnableMotor(axis_name, false);
//         delay(3000);
//     }
// }
