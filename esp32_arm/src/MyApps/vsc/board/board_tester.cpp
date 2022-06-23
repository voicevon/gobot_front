#include "board_tester.h"


void Vsc_BoardTest::LinkBoard(CncBoardBase* board){
    Serial.println("[Info] Vsc_BoardTest::LinkBoard()");
    this->__board = (Vsc_Board*) (board);
    this->LinkCncBoard(board);
}


void Vsc_BoardTest::Test_Motor(int loop_count){

}



void Vsc_BoardTest::Test_AngleSensor(int loop_count){
    MagneticSensorAnalog* sensor = this->__board->GetSensor();

    for(int i=0; i < loop_count; i++){
        Serial.print(sensor->getAngle());
    }

}