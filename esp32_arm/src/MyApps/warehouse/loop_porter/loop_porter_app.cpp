
#include "loop_porter_app.h"
#include <HardwareSerial.h>


Twh_LoopPorter_App::Twh_LoopPorter_App(int row_id){
    this->__robot_row_id = row_id;
    Serial.print("\n[Info] Twh_LoopPorter_App::Twh_LoopPorter_App() is constructed");
}

void Twh_LoopPorter_App::ExecuteCommand(GcodeText* gcode_text){
    const char command_test_led[8] = {"app:led"};

    Logger::Info("Twh_LoopPorter_App::ExecuteCommand()");
    gcode_text->PrintFlat("Twh_LoopPorter_App::ExecuteCommand()");

    if (gcode_text->IsEqualTo(command_test_led)){
        for (int i=0; i<200; i++){
            __board->TestLeds(200);
        }
    }
}





