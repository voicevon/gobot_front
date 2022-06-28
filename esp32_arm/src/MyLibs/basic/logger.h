#pragma once

#include "MyBoards/const.h"

class Logger{
    public :
        static void PrintTitle(const char * title);
        static void Warn(const char * title);
        static void Error(const char * title);
};


