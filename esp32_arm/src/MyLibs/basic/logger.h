#pragma once

#include "MyBoards/const.h"

class Logger{
    public :
        static void Info(const char * title);
        static void Debug(const char * title);
        static void Warn(const char * title);
        static void Error(const char * title);
        static void Halt(const char* title);
};


