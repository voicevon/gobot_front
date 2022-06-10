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
