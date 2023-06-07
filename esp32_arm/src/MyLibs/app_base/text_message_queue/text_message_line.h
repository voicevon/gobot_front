#pragma once
#include "MyLibs/basic/c_string/c_string_base.h"

#define TEXT_MESSAGE_LINE_MAX_CHARS_COUNT 50

class TextMessageLine: public C_String_Base{
    public:
        enum Enum_Category{
            LUA,
            GCODE,
            APP,
        };

        TextMessageLine(): C_String_Base(TEXT_MESSAGE_LINE_MAX_CHARS_COUNT, __all_chars){};
        // TextMessageLine(Enum_Category preset_category): C_String_Base(50, __all_chars){__preset_category = preset_category;};
        Enum_Category GetCategory();
        void RemovePrefix();

    private:
        char __all_chars[TEXT_MESSAGE_LINE_MAX_CHARS_COUNT];
        Enum_Category __category;
        Enum_Category __preset_catetory;
};