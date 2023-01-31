#pragma once

#include "my_const.h"
#include <HardwareSerial.h>
#include "arduinotrace.h"  //https://www.youtube.com/watch?v=JHMpszgzWSg

/*
#define TRACE() \
    Serial.print(__FILE__); \
    Serial.print(":"); \
    Serial.print(__LINE__); \
    Serial.print(":"); \
    Serial.print(__FUNCTION__); \
    Serial.print(":"); \
    Serial.print(__PRETTY_FUNCTION__); \
    Serial.flush();
*/
// class LoggerStyle{
//     public:
//         int fore_color;
//         int back_color;
//         bool reset_in_the_end;
// };


enum EnumStyle{
    Error = 1,
    Warning =2,
    Info = 3,
    Debug =4,
};
enum EnumStyle_position{
    Begin =1,
    End = 2,
};

// class LoggerHelper{
// };

class Logger{
    public :
        static void SetStyle(EnumStyle style_name, EnumStyle_position style_position);
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
        static void Print(const char* var_name, int64_t value);
        static void Print(const char* var_name, double value);
        static void Print(const char* var_name, const char* value);
        static void Print(const char* var_name, const char value);

        static void __print_sid();
        static int sid;
        // static LoggerStyle style_title;
        // static LoggerStyle style_key_value;
};


