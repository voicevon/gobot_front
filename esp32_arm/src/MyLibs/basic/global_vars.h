#pragma once

#include "WString.h"

class GlobalVars{
    public:
        static GlobalVars& Instance(){
            static GlobalVars instance;
            return instance;
        }
        String text;
};