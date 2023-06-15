#pragma once

// #include "my_const.h"
// #include <HardwareSerial.h>
// #include "arduinotrace.h"  //https://www.youtube.com/watch?v=JHMpszgzWSg

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


// enum EnumStyle{
//     Error = 1,
//     Warning =2,
//     Info = 3,
//     Debug =4,
// };
// enum EnumStyle_position{
//     Title_Begin =1,
//     Window_Begin = 2,
//     Window_End = 3,
// };


extern void InitMonitorUart();

extern void logWarn(const char* title);
extern void logInfo(const char* title);
extern void logDebug(const char* title);

extern void logPrint_Char(const char* title, const char* text);
extern void logPrint_Int(const char* title, int v);

// class LoggerHelper{
// };

// class Logger{
//     public :
//         static void SetStyle(EnumStyle style_name, EnumStyle_position style_position);
//         static void Info(const char * title);
//         // template <typename anyPrintable>
//         // static void Info(const char * title, anyPrintable var, anyPrintable value);
//         static void Debug(const char * title);
//         static void Warn(const char * title);
//         static void Error(const char * title);
//         static void Halt(const char* title);

//         // template <typename anyPrintable>
//         // static void Print(const char* var_name, anyPrintable value);
//         static void Print(const char* var_name, const bool value);
//         static void Print(const char* var_name, const float value);
//         static void Print(const char* var_name, const int32_t value);
//         static void Print(const char* var_name, const uint32_t value);
//         static void Print(const char* var_name, const int64_t value);
//         static void Print(const char* var_name, const unsigned long value);
//         static void Print(const char* var_name, const long value);
//         static void Print(const char* var_name, const double value);

//         //will print a string.
//         static void Print(const char* var_name, const char* chars);
//         // static void Print(const char* var_name, const char*& chars);
//         // static void Print(const char* var_name, const char chars[]);
//         //will print single char.
//         static void Print(const char* var_name, const char single_char);

//         static void __print_sid();
//         static int sid;
//         // static LoggerStyle style_title;
//         // static LoggerStyle style_key_value;
// };



