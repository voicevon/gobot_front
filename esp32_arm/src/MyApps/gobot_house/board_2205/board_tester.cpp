#include "board_tester.h"


void GobotHouse_BoardTest::LinkBoard(CncBoardBase* board){
    Serial.println("[Info] GobotHouse_BoardTest::LinkBoard()");
    this->__board = (Board_GobotHouse*) (board);
    this->LinkCncBoard(board);
}

// void GobotHouse_BoardTest::Test_EefLoadUnload(int loop_count){
//     if(loop_count ==0 ) return;
//     Serial.println ("[Info] GobotHouse_BoardTest::Test_EefLoadUnload()  ");
//     RobotEef_GobotHouse* eef = this->__board->GetEef();
//     for (int i=0; i<loop_count; i++){
//         Serial.print("Eef Load   ");
//         eef->Run(EEF_CODE_LOAD);
//         delay(3000);
//         Serial.println("Eef Unload");
//         eef->Run(EEF_CODE_UNLOAD);
//         delay(3000);
//     }
//     eef->Run(EEF_CODE_SLEEP);
// }

