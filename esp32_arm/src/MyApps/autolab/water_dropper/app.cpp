
#include "app.h"
#include <HardwareSerial.h>


WaterDropper_App::WaterDropper_App(){
    // this->__robot_row_id = row_id;
    Serial.print("\n[Info] WaterDropper_App::WaterDropper_App() is constructed");
}

void WaterDropper_App::ExecuteCommand(TextMessageLine* gcode_text){
    const char command_test_led[8] = {"app:led"};

    Logger::Info("WaterDropper_App::ExecuteCommand()");
    // gcode_text->PrintFlat("WaterDropper_App::ExecuteCommand()");

    // if (gcode_text->IsEqualTo(command_test_led)){
    //     for (int i=0; i<200; i++){
    //         __board->TestLeds(200);
    //     }
    // }
}





