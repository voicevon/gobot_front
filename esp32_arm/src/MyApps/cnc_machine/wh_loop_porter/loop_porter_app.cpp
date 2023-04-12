
#include "loop_porter_app.h"
#include <HardwareSerial.h>


Twh_LoopPorter_App::Twh_LoopPorter_App(int row_id){
    this->__robot_row_id = row_id;
    Serial.print("\n[Info] Twh_LoopPorter_App::Twh_LoopPorter_App() is constructed");
}

void Twh_LoopPorter_App::ExecuteCommand(const char* command){
    Logger::Info("Twh_LoopPorter_App::ExecuteCommand");
    Logger::Print("command", command);
}





