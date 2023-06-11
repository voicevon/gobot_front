#pragma once

#include "c_string_base.h"

class FsFilename: public C_String_Base{
    public:
        FsFilename(): C_String_Base(__all_chars,30){};
        
    private:
        char __all_chars[30];
};