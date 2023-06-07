#pragma once
#include "MyLibs/basic/c_string/c_string_base.h"



class CommandText: public C_String_Base{
    public:
        enum Enum_Category{
            LUA,
            GCODE,
            APP,
        };

        CommandText(): C_String_Base(50, __all_chars){};
        // CommandText(Enum_Category preset_category): C_String_Base(50, __all_chars){__preset_category = preset_category;};
        Enum_Category GetCategory();
        // const char* GetChars(){return __all_chars;};

    private:
        char __all_chars[50];
        Enum_Category __category;
        Enum_Category __preset_catetory;
};