#include "board_tester.h"
#include "FastAccelStepper.h"
// #include "ESP32Step/src/TeensyStep.h" 

void GobotHouse_2206_BoardTest::LinkBoard(CncBoardBase* board){
    Serial.println("[Info] GobotHouse_2206_BoardTest::LinkBoard()");
    this->__board = (Board_GobotHouse_2206*) (board);
    this->LinkCncBoard(board);
}


void GobotHouse_2206_BoardTest::Test_EefLoadUnload(int loop_count){
    if(loop_count ==0 ) return;
    Serial.println ("[Info] GobotHouse_2206_BoardTest::Test_EefLoadUnload()  ");
    RobotEef_GobotHouse* eef;  // todo: where is the instance?
    for (int i=0; i<loop_count; i++){
        Serial.print("Eef Load   ");
        eef->Run(EEF_CODE_LOAD);
        delay(3000);
        Serial.println("Eef Unload");
        eef->Run(EEF_CODE_UNLOAD);
        delay(3000);
    }
    eef->Run(EEF_CODE_SLEEP);
}


// void GobotHouse_2206_BoardTest::Test_StepperDriver_OnAlpha(int loop_count){
//     if(loop_count ==0 ) return;
//     Serial.println ("[Info] GobotHouse_2206_BoardTest::Test_StepperDriver_OnAlpha()  ");
//     Stepper* stepper = this->__board->GetStepperDriver_OnAlpha();
//     // StepControl control;
//     this->__board->EnableMotor(AXIS_ALPHA, true);
//     stepper->setMaxSpeed(6000);
//     stepper->setAcceleration(6000);

//     for (int i=0; i<loop_count; i++){
//         Serial.print("   CW");
//         stepper->setTargetRel(-50000);
//         // this->__stepControl->move(*stepper);
//         // control.move(*stepper);
//         Serial.print("  paused");
//         delay(2000);

//         Serial.print("   CCW");
//         stepper->setTargetAbs(0);
//         // this->__stepControl->move(*stepper);
//         // control.move(*stepper);
//         Serial.println("  paused");
//         delay(2000);
//     }
// }

// void GobotHouse_2206_BoardTest::Test_ServoDriver_OnBeta(int loop_count){
//     if(loop_count ==0 ) return;
//     Serial.println ("[Info] GobotHouse_2206_BoardTest::Test_ServoDriver()  ");
//     Servo* servo = this->__board->GetServoDriver_OnBeta();
//     for (int i=0; i<loop_count; i++){
//         Serial.print("Servo angle  50   ");
//         servo->write(50);
//         delay(2000);
//         Serial.print("  90   ");
//         servo->write(90);
//         delay(2000);
//         Serial.println("  180   ");
//         servo->write(180);
//         delay(2000);
//     }
// }

