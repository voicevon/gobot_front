#include "board_test_cnc.h"


void AsarAgv_BoardTest::LinkBoard(CncBoardBase* board){
    Serial.println("[Info] AsarAgv_BoardTest::LinkBoard()");
    // this->__board = (CncBoardBase*) (board);
    // this->LinkCncBoard(board);
}

// void AsarAgv_BoardTest::Test_EefUpDown(int loop_count){
//     if(loop_count ==0 ) return;
//     Serial.println ("[Info] AsarAgv_BoardTest::Test_room_sensors()  ");
//     RobotEef_GobotHouse* eef = this->__board->GetEef();
//     for (int i=0; i<loop_count; i++){
//         Serial.print("Eef UP   ");
//         eef->Run(EEF_CODE_UP);
//         delay(3000);
//         Serial.println("Eef Down");
//         eef->Run(EEF_CODE_DOWN);
//         delay(3000);
//     }
// }

// void AsarAgv_BoardTest::Test_EefLoadUnload(int loop_count){
//     if(loop_count ==0 ) return;
//     Serial.println ("[Info] AsarAgv_BoardTest::Test_room_sensors()  ");
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



// void xx(){
//     SingleAxisHomer* hz = this->__board->GetHomer('Z');
//     SingleAxisHomer* hy = this->__board->GetHomer('Y');
//     bool zt,yt;
//     for(int i=0; i<loop_count; i++){
//         zt= hz->IsTriged();
//         yt = hy->IsTriged();
//         Serial.print("z: ");
//         Serial.print(zt);
//         Serial.print("\ty: ");
//         Serial.println(yt);
//         delay(200);
//     }
// }