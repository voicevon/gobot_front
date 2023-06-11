// #include "gcode_text.h"
#include "HardwareSerial.h"
#include "MyLibs/utility/logger.h"
#include "c_string_base.h"




void C_String_Base::Init(char* buffer, int buffer_size){
    __buffer_size = buffer_size;
    __text = buffer;
    // this->CopyFrom(command);
}

C_String_Base::C_String_Base(char* buffer,int buffer_size){
    __buffer_size = buffer_size;
    __text = buffer;
    // this->CopyFrom(command);
}

//TODO:  be protected,  and  check if or not there is 0x00 in then command.

int C_String_Base::CopyFrom(const char* text){
    for(int i=0; i<__buffer_size; i++){
        __text[i] = text[i];
        // Logger::Print("char", __text[i]);
        if(text[i] == 0x00){
            //end of string.
            // this->PrintFlat("caller::C_String_Base::CopyFrom(const char* command)");
            return 1;
        }
    }
    Logger::Error("C_String_Base::CopyFrom()  oversize" );
}

void C_String_Base::RemovePrefix(const char prefix_ender){
    bool got_prefix_ender = false;
    int prefix_position;
    for(int i=0; i<__text_length;i++){
        if (got_prefix_ender){
            __text[i-prefix_position] = __text[i];
        }else{
            if (__text[i]== prefix_ender){
                got_prefix_ender = true;
                prefix_position = i;
            }
        }
    }
}

int C_String_Base::CopyFrom(const char* text, int length){
    if (length >= __buffer_size){
        Logger::Error("C_String_Base::ReConstruct()  oversize ");
        return C_STRING_ERR_OVER_SIZE;
    }
    int i;
    for(i=0; i<length; i++){
        this->__text[i] = text[i];
        // Logger::Print("char", this->__text[i]);
    }
    this->__text[i] = 0x00;
    __text_length = length;
    // Logger::Debug("C_String_Base::CopyFrom()");
    // Logger::Print("C_String_Base::CopyFrom()  length", length);
    // this->PrintFlat("C_String_Base::CopyFrom()");
    return C_STRING_OK;

}

void C_String_Base::CopyTo(char* destination){
    // #warning __FILE__  "[" __LINE__ "] void C_String_Base::CopyTo(char* destination) is depricated, use CopyTo(char, int)"
    #pragma __FILE__  "[" __LINE__ "] void C_String_Base::CopyTo(char* destination) is depricated, use CopyTo(char, int)"
    for(int i=0; i<__text_length; i++){
        destination[i] = this->__text[i];
        if(destination[i] == 0x00){
            //end of string.
            return;
        }
    }
}

void C_String_Base::_CopyTo(char* destination, int remove_prefix_bytes_length){
    for(int i=0; i<__text_length; i++){
        destination[i] = this->__text[i + remove_prefix_bytes_length];
        if(destination[i] == 0x00){
            //end of string.
            return;
        }
    }
}

void C_String_Base::Concat(const char* text){
    int origin_length = __text_length;
    for(int i=0; i<__buffer_size; i++){
        __text[i + origin_length] = *(text + i );
        __text_length++;
        if (__text[i] == 0x00){
            return;
        }
    }  
    Logger::Error("C_String_Base::Concat()  oversize");
}

void C_String_Base::PrintFlat(const char* title){
    Logger::Info(title);
    // Logger::Print("chars len",this->__buffer_size );
    
    for(int i=0; i<__buffer_size; i++){
        if (this->__text[i] == 0x00) {
            Serial.print(FORE_YELLOW);
            Serial.print("<<<\n");
            return;
        }
        Serial.print(this->__text[i]);
    }
}

bool C_String_Base::IsEqualTo(const char* chars){
    for(int i=0; i<__buffer_size; i++){
        if (this->__text[i] != chars[i]){
            return false;
        }
        if(this->__text[i] == 0x00){
            return true;
        }
    }
    return false;
}

bool C_String_Base::IsPrefix(const char* chars){
    for(int i=0; i<__buffer_size; i++){
        Logger::Print("chars[i]---", chars[i]);
        Logger::Print("this->__text[i]---", this->__text[i]);
        if (this->__text[i] != chars[i]){
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

int C_String_Base::Find_MismachLocation(const void* b, int length){
    uint8_t* aa = (uint8_t*) __text;
    uint8_t* bb = (uint8_t*) b;
    for(int i=0; i<length ; i++){
        if (*(aa+i) != *(bb+i)){
            return i;
        }
    }
    return -1;
}