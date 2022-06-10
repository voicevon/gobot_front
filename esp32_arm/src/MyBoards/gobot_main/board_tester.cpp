#include "board_tester.h"


void GobotMain_BoardTest::LinkBoard(CncBoardBase* board){
    Serial.println("[Info] GobotMain_UnitTest::LinkBoard()");
    this->__board = (Board_GobotMain*) (board);
    this->LinkCncBoard(board);
}

void GobotMain_BoardTest::Test_room_sensors(int loop_count){
    Serial.println ("[Info] GobotMain_UnitTest::Test_room_sensors()  ");
    delay(3000);
    uint8_t rooms;
    for (int i=0; i<loop_count; i++){
        rooms =  this->__board->ReadAllRooms();
        Serial.println(rooms, 'B');

    }
}


void GobotMain_BoardTest::Test_EefUpDown(int loop_count){
    if(loop_count ==0 ) return;
    Serial.println ("[Info] GobotMain_UnitTest::Test_room_sensors()  ");
    RobotEef_GobotMain* eef = this->__board->GetEef();
    for (int i=0; i<loop_count; i++){
        Serial.print("Eef UP   ");
        eef->Run(EEF_CODE_UP);
        delay(3000);
        Serial.println("Eef Down");
        eef->Run(EEF_CODE_DOWN);
        delay(3000);
    }
}

void GobotMain_BoardTest::Test_EefLoadUnload(int loop_count){
    if(loop_count ==0 ) return;
    Serial.println ("[Info] GobotMain_UnitTest::Test_room_sensors()  ");
    RobotEef_GobotMain* eef = this->__board->GetEef();
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

