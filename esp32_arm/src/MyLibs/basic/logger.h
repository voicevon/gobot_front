#pragma once

#include "MyBoards/const.h"

class Logger{
    public :
        static void Info(const char * title);
        // template <typename anyPrintable>
        // static void Info(const char * title, anyPrintable var, anyPrintable value);
        static void Debug(const char * title);
        static void Warn(const char * title);
        static void Error(const char * title);
        static void Halt(const char* title);

        // template <typename anyPrintable>
        // static void Print(const char* var_name, anyPrintable value);
        static void Print(const char* var_name, bool value);
        static void Print(const char* var_name, float value);
        static void Print(const char* var_name, int32_t value);
        static void Print(const char* var_name, uint32_t value);
        static void Print(const char* var_name, double value);
        static void Print(const char* var_name, const char* value);
        static void Print(const char* var_name, const char value);
};


