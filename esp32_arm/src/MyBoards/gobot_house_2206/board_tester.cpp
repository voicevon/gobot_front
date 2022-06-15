#include "board_tester.h"


void GobotHouse_2206_BoardTest::LinkBoard(CncBoardBase* board){
    Serial.println("[Info] GobotHouse_2206_BoardTest::LinkBoard()");
    this->__board = (Board_GobotHouse_2206*) (board);
    this->LinkCncBoard(board);
}


void GobotHouse_2206_BoardTest::Test_EefLoadUnload(int loop_count){
    if(loop_count ==0 ) return;
    Serial.println ("[Info] GobotHouse_2206_BoardTest::Test_EefLoadUnload()  ");
    RobotEef_GobotHouse* eef = this->__board->GetEef();
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

