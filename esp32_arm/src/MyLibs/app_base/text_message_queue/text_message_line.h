#pragma once
#include "von/cpp/basic/c_string/c_string_base.h"

#define TEXT_MESSAGE_LINE_MAX_CHARS_COUNT 50

class TextMessageLine: public C_String_Base{
    public:
        enum Enum_Category{
            GCODE,  //default
            LUA,
            FILE,
            // UNKNOWN
        };

        TextMessageLine(): C_String_Base(__all_chars, TEXT_MESSAGE_LINE_MAX_CHARS_COUNT){};
        void SetCatogory(TextMessageLine::Enum_Category previous_category ){
                __category=previous_category;
             };
        bool IsCategoryUpdated();
        Enum_Category GetCategory();

    private:
        char __all_chars[TEXT_MESSAGE_LINE_MAX_CHARS_COUNT];
        Enum_Category __category;
        // Enum_Category __preset_catetory;
};