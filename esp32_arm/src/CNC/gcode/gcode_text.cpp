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
        __chars[i] = command[i];
        // Logger::Print("char", __chars[i]);
        if(command[i] == 0x00){
            //end of string.
            // this->PrintFlat("caller::GcodeText::CopyFrom(const char* command)");
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
        // Logger::Print("char", this->__chars[i]);
    }
    this->__chars[i] = 0x00;
    // Logger::Debug("GcodeText::CopyFrom()");
    // Logger::Print("GcodeText::CopyFrom()  length", length);
    // this->PrintFlat("GcodeText::CopyFrom()");
    return GCODE_TEXT_OK;

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

void GcodeText::PrintFlat(const char* title){
    Logger::Info(title);
    // Logger::Print("chars len",this->__length );
    
    for(int i=0; i<REPRAP_GCODE_MAX_SIZE; i++){
        if (this->__chars[i] == 0x00) {
            Serial.print(FORE_YELLOW);
            Serial.print("<<<\n");
            return;
        }
        Serial.print(this->__chars[i]);
    }
}


bool GcodeText::IsEqualTo(const char* chars){
    for(int i=0; i<REPRAP_GCODE_MAX_SIZE; i++){
        if (this->__chars[i] != chars[i]){
            return false;
        }
        if(this->__chars[i] == 0x00){
            return true;
        }
    }
    return false;
}

bool GcodeText::IsPrefix(const char* chars){
    for(int i=0; i<REPRAP_GCODE_MAX_SIZE; i++){
        Logger::Print("chars[i]---", chars[i]);
        Logger::Print("this->__chars[i]---", this->__chars[i]);
        if (this->__chars[i] != chars[i]){
            if(chars[i] == 0x00){
                Logger::Print("GcodeText::IsPrefix() return true", 92);
                return true;
            }
            Logger::Print("GcodeText::IsPrefix() return false", 91);
            return false;
        }
    }
    Logger::Print("GcodeText::IsPrefix() return true", 93);
    return false;
}

