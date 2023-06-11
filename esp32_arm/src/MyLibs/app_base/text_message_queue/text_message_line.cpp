#include "text_message_line.h"


bool TextMessageLine::IsCategoryUpdated(){
    if (IsPrefix("dofile:") || IsPrefix("dostring:")){
        __category = LUA;
        return true;
    }else if (this->IsPrefix("gcode:")){
        __category = GCODE;
        return true;
    }else if (IsPrefix("file_open:") || IsPrefix("file_close:") || IsPrefix("file_line")){
        __category = FILE;
        return true;
    }
    return false;
}


TextMessageLine::Enum_Category TextMessageLine::GetCategory(){

}
