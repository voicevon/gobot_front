#include "board_tester.h"


void GobotMain_BoardTest::LinkBoard(CncBoardBase* board){
    Serial.println("[Info] GobotMain_BoardTest::LinkBoard()");
    this->__board = (Board_GobotMain*) (board);
    this->LinkCncBoard(board);
}

void GobotMain_BoardTest::Test_room_sensors(int loop_count){
    Serial.println ("[Info] GobotMain_BoardTest::Test_room_sensors()  ");
    delay(3000);
    uint8_t rooms;
    for (int i=0; i<loop_count; i++){
        rooms =  this->__board->ReadAllRooms();
        Serial.println(rooms, BIN);
        delay(1000);
    }
}


void GobotMain_BoardTest::Test_EefUpDown(int loop_count){
    if(loop_count ==0 ) return;
    Serial.println ("[Info] GobotMain_BoardTest::Test_EefUpDown()  ");
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
    Serial.println ("[Info] GobotMain_BoardTest::Test_EefLoadUnload()  ");
    RobotEef_GobotMain* eef = this->__board->GetEef();
    eef->Run(EEF_CODE_UP);
    for (int i=0; i<loop_count; i++){
        Serial.print("Eef Load   ");
        eef->Run(EEF_CODE_LOAD);
        delay(2000);
        Serial.println("Eef Unload");
        eef->Run(EEF_CODE_UNLOAD);
        delay(1000);
        Serial.println("Eef Sleep");

        eef->Run(EEF_CODE_SLEEP);
        delay(3000);
    }
}

void GobotMain_BoardTest::Test_Eef_Full(int loop_count){
    if(loop_count ==0 ) return;
    Serial.println ("[Info] GobotMain_BoardTest::Test_Eef_Full()  ");
    RobotEef_GobotMain* eef = this->__board->GetEef();
    for (int i=0; i<loop_count; i++){
        Serial.print("Down   ");
        eef->Run(EEF_CODE_DOWN);
        delay(400);

        Serial.print("Load  ");
        eef->Run(EEF_CODE_LOAD);
        delay(1000);

        Serial.print("Up  ");
        eef->Run(EEF_CODE_UP);
        delay(1000);
        
        Serial.print("Down  ");
        eef->Run(EEF_CODE_DOWN);
        delay(400);

        Serial.print("Unload  ");
        eef->Run(EEF_CODE_UNLOAD);
        delay(1000);

        Serial.print("Up  ");
        eef->Run(EEF_CODE_UP);
        delay(1000);

        Serial.println("Eef Sleep");
        eef->Run(EEF_CODE_SLEEP);
        delay(5000);
    }
}
