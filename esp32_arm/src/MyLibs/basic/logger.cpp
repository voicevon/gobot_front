#include "logger.h"
#include <HardwareSerial.h>

void Logger::Info(const char* title){
    Serial.print(FORE_YELLOW);
    Serial.print(BGC_BLUE);
    Serial.print("[Info] ");
    Serial.print(title);
    // Serial.print("  ");
    // Serial.print(value);
    Serial.print(BGC_BLACK);
    Serial.println(FORE_GREEN);
}
// template <typename anyPrintable>
// void Logger::Info(const char* title, anyPrintable var, anyPrintable value){
//     Serial.print(FORE_YELLOW);
//     Serial.print(BGC_BLUE);
//     Serial.print("[Info] ");
//     Serial.print(title);
//     Serial.print("  ");
//     Serial.print(value);
//     Serial.print(BGC_BLACK);
//     Serial.println(FORE_GREEN);
// }

void Logger::Debug(const char* title){
    Serial.print(FORE_YELLOW);
    Serial.print(BGC_BLUE);
    Serial.print("[Debug] ");
    Serial.print(title);
    Serial.print(BGC_BLACK);
    Serial.println(FORE_PINK);
}

void Logger::Warn(const char * title){
    Serial.print(FORE_YELLOW);
    Serial.print(BGC_BLUE);
    Serial.print("[Warn] ");
    Serial.print(title);
    Serial.print(BGC_BLACK);
    Serial.println(FORE_YELLOW);
}

void Logger::Error(const char * title){
    Serial.print(FORE_RED);
    Serial.print(BGC_BLUE);
    Serial.print("[Error]");
    Serial.print(title);
    Serial.print(BGC_BLACK);
    Serial.println(FORE_RED); 
}

void Logger::Halt(const char* title){
    Serial.print(FORE_YELLOW);
    Serial.print(BGC_BLUE);
    Serial.print("[Halt]");
    Serial.print(title);
    Serial.print(BGC_BLACK);
    Serial.println(FORE_GREEN);
    Serial.print("Esp32 is halt  ");
    while (true){
        Serial.print("= ");
        delay(500);
    }
}

// template <typename anyPrintable>
// void Logger::Print(const char* var_name, anyPrintable value){
//     Serial.print(var_name);
//     Serial.print(value);
// }

void Logger::Print(const char* var_name, bool value){
    Serial.print(var_name);
    Serial.print("=\t");
    Serial.println(value);
}

void Logger::Print(const char* var_name, float value){
    Serial.print(var_name);
    Serial.print("=\t");
    Serial.println(value);
}

void Logger::Print(const char* var_name, const char* value){
    Serial.print(var_name);
    Serial.print("=\t");
    Serial.println(value);
}

void Logger::Print(const char* var_name, double value){
    Serial.print(var_name);
    Serial.print("=\t");
    Serial.println(value);
}

void Logger::Print(const char* var_name, int32_t value){
    Serial.print(var_name);
    Serial.print(" =\t");
    Serial.println(value);
}