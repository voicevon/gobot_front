#include "gcode_text.h"
#include "HardwareSerial.h"
#include "MyLibs/basic/logger.h"


GcodeText::GcodeText(){
    this->bytes[0] = 0x00;
}

GcodeText::GcodeText(const char* command){
    this->ReConstruct(command);
}

int GcodeText::ReConstruct(const char* command){
    for(int i=0; i<REPRAP_GCODE_MAX_SIZE; i++){
        this->bytes[i] = command[i];
        // Logger::Print("char", this->bytes[i]);
        if(command[i] == 0x00){
            //end of string.
            // Logger::Print("GcodeText::ReConstruct() got ender", 91);
            // this->PrintFlat();
            return 1;
        }
    }
}

int GcodeText::ReConstruct(const char* command, int length){

}

void GcodeText::PrintFlat(){
    for(int i=0; i<REPRAP_GCODE_MAX_SIZE; i++){
        if (this->bytes[i] == 0x00) {
            Serial.print("<<<");
            return;
        }
        Serial.print(this->bytes[i]);

    }
}
