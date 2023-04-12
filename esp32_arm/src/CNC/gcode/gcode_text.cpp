#include "gcode_text.h"

GcodeText::GcodeText(){
    this->bytes[0] = 0x00;
}

GcodeText::GcodeText(const char* command){
    for(int i=0; i<REPRAP_GCODE_MAX_SIZE; i++){
        this->bytes[i] = command[i];
        if(command[i] == 0x00){
            //end of string.
            return;
        }
    }
}

void GcodeText::ReConstruct(const char* command, int length){
    int i;
    for(i=0; i<length; i++){
        this->bytes[i] = command[i];
    }
    this->bytes[i] = 0x00;
}
