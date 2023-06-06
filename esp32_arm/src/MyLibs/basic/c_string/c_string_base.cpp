// #include "gcode_text.h"
#include "HardwareSerial.h"
#include "MyLibs/utility/logger.h"
#include "c_string_base.h"


// C_String_Base::C_String_Base(int buffer_size){
//     this->__chars[0] = 0x00;
//     __length = buffer_size;
// }

C_String_Base::C_String_Base(int buffer_size, char* buffer){
    __length = buffer_size;
    __chars = buffer;
    // this->CopyFrom(command);
}

int C_String_Base::CopyFrom(const char* command){
    for(int i=0; i<REPRAP_GCODE_MAX_SIZE; i++){
        __chars[i] = command[i];
        // Logger::Print("char", __chars[i]);
        if(command[i] == 0x00){
            //end of string.
            // this->PrintFlat("caller::C_String_Base::CopyFrom(const char* command)");
            return 1;
        }
    }
}

int C_String_Base::CopyFrom(const char* command, int length){
    if (length >= REPRAP_GCODE_MAX_SIZE){
        Logger::Error("C_String_Base::ReConstruct()  oversize ");
        return C_STRING_ERR_OVER_SIZE;
    }
    int i;
    for(i=0; i<length; i++){
        this->__chars[i] = command[i];
        // Logger::Print("char", this->__chars[i]);
    }
    this->__chars[i] = 0x00;
    // Logger::Debug("C_String_Base::CopyFrom()");
    // Logger::Print("C_String_Base::CopyFrom()  length", length);
    // this->PrintFlat("C_String_Base::CopyFrom()");
    return C_STRING_OK;

}

void C_String_Base::CopyTo(char* destination){
    #warning __FILE__  "[" __LINE__ "] void C_String_Base::CopyTo(char* destination) is depricated, use CopyTo(char, int)"
    #pragma __FILE__  "[" __LINE__ "] void C_String_Base::CopyTo(char* destination) is depricated, use CopyTo(char, int)"
    for(int i=0; i<REPRAP_GCODE_MAX_SIZE; i++){
        destination[i] = this->__chars[i];
        if(destination[i] == 0x00){
            //end of string.
            return;
        }
    }
}
void C_String_Base::CopyTo(char* destination, int remove_prefix_bytes_length){
    for(int i=0; i<REPRAP_GCODE_MAX_SIZE; i++){
        destination[i] = this->__chars[i + remove_prefix_bytes_length];
        if(destination[i] == 0x00){
            //end of string.
            return;
        }
    }
}


void C_String_Base::PrintFlat(const char* title){
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


bool C_String_Base::IsEqualTo(const char* chars){
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

bool C_String_Base::IsPrefix(const char* chars){
    for(int i=0; i<REPRAP_GCODE_MAX_SIZE; i++){
        Logger::Print("chars[i]---", chars[i]);
        Logger::Print("this->__chars[i]---", this->__chars[i]);
        if (this->__chars[i] != chars[i]){
            if(chars[i] == 0x00){
                Logger::Print("C_String_Base::IsPrefix() return true", 92);
                return true;
            }
            Logger::Print("C_String_Base::IsPrefix() return false", 91);
            return false;
        }
    }
    Logger::Print("C_String_Base::IsPrefix() return true", 93);
    return false;
}

