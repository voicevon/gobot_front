#pragma once

#include "c_string_base.h"

class FsFilename: public C_String_Base{
    public:
        FsFilename(): C_String_Base(30, __all_chars){};
        
    private:
        char __all_chars[30];
};