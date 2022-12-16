
#include "teeth_wh_app.h"
#include <HardwareSerial.h>


Twh2_App::Twh2_App(){
    Serial.print("\n[Info] Twh2_App::Twh2_App() is constructed");
}



void Twh2_App::AsyncExecuteMqttCommand(const char* command){
    String str_command = String(command);
    // Logger::Info("Twh2_App::ExecuteMqttCommand()");
    bool debug = false;
    if (debug){
        Logger::Print("command",str_command);
        Logger::Print("substring(0,7)", str_command.substring(0,7));
        Logger::Print("substring(7)", str_command.substring(7));
        Serial.print(str_command);
        Serial.print("\t");
        Serial.print(str_command.substring(0,7));
        Serial.print("\t");
        Serial.print(str_command.substring(7));
        Serial.println();
    }
    Gcode my_gcode = Gcode(command);
    this->_gcode_queue->AppendGcodeCommand(command);

        // Complete command example:  
        // payload: "twh_store R12C0"      //store to row=12, col=0
        // payload: "twh_retrv R15C7"   // retrieve from row=15, col=7
    // if (str_command.substring(0,9) == "twh_store"){
    //     // This is a robot command, not a gcode or mcode
    //     str_command = str_command.substring(9);
    //     int row = my_gcode.get_value('R');   
    //     int col = my_gcode.get_value('C');
    //     Logger::Print("row", row);
    //     Logger::Print("col", col);
    //     this->__robot->StoreToCell(row, col);
    // } else if (str_command.substring(0,9) == "twh_retrv"){
    //     // This is a robot command, not a gcode or mcode
    //     str_command = str_command.substring(9);
    //     int row = my_gcode.get_value('R');   
    //     int col = my_gcode.get_value('C');
    //     Logger::Print("row", row);
    //     Logger::Print("col", col);
    //     this->__robot->RetrieveFromCell(row, col);
    // }else{
    //     // this is a normal gcode.
    //     this->_gcode_queue->AppendGcodeCommand(command);

    // }

}




