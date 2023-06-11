#include "text_message_line.h"




TextMessageLine::Enum_Category TextMessageLine::GetCategory(){
    if (this->IsPrefix("lua:")){
        __category = LUA;
    }else if (this->IsPrefix("gcode:")){
        __category = GCODE;
    }else if (this->IsPrefix("app:")){
        __category = FILE;
    }else{
        __category = __preset_catetory;
    }
}
