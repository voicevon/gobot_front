#include "gcode_text.h"
#include "HardwareSerial.h"
#include "MyLibs/basic/logger.h"


GcodeText::GcodeText(){
    this->__chars[0] = 0x00;
}

GcodeText::GcodeText(const char* command){
    this->CopyFrom(command);
}

int GcodeText::CopyFrom(const char* command){
    for(int i=0; i<REPRAP_GCODE_MAX_SIZE; i++){
        this->__chars[i] = command[i];
        // Logger::Print("char", this->bytes[i]);
        if(command[i] == 0x00){
            //end of string.
            // Logger::Print("GcodeText::ReConstruct() got ender", 91);
            // this->PrintFlat();
            return 1;
        }
    }
}

int GcodeText::CopyFrom(const char* command, int length){
    if (length >= REPRAP_GCODE_MAX_SIZE){
        Logger::Error("GcodeText::ReConstruct()  oversize ");
        return GCODE_TEXT_ERR_OVER_SIZE;
    }
    int i;
    for(i=0; i<length; i++){
        this->__chars[i] = command[i];
        // Logger::Print("char", this->bytes[i]);
            return GCODE_TEXT_OK;
        }
    this->__chars[i] = 0x00;
}

void GcodeText::CopyTo(char* destination){
    for(int i=0; i<REPRAP_GCODE_MAX_SIZE; i++){
        destination[i] = this->__chars[i];
        if(destination[i] == 0x00){
            //end of string.
            return;
        }
    }
}


void GcodeText::PrintFlat(){
    for(int i=0; i<REPRAP_GCODE_MAX_SIZE; i++){
        if (this->__chars[i] == 0x00) {
            Serial.print("<<<");
            return;
        }
        Serial.print(this->__chars[i]);

    }
}
