#include "unit_test.h"


void GobotMain_UnitTest::LinkBoard(CncBoardBase* board){
    this->__board = (Board_GobotMain*) (board);
}

void GobotMain_UnitTest::Test_room_sensors(int loop_count){
    Serial.println ("[Info] GobotMain_UnitTest::Test_room_sensors()  ");
    delay(3000);
    uint8_t rooms;
    for (int i=0; i<loop_count; i++){
        rooms =  this->__board->ReadAllRooms();
        Serial.println(rooms, 'B');

    }
}
