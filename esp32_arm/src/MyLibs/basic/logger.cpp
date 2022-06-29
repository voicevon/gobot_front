#include "logger.h"
#include <HardwareSerial.h>


void Logger::Info(const char* title){
    Serial.print(FORE_YELLOW);
    Serial.print(BGC_BLUE);
    Serial.print(title);
    Serial.print(BGC_BLACK);
    Serial.println(FORE_GREEN);
}

void Logger::Debug(const char* title){
    Serial.print(FORE_YELLOW);
    Serial.print(BGC_BLUE);
    Serial.print(title);
    Serial.print(BGC_BLACK);
    Serial.println(FORE_GREEN);
}

void Logger::Warn(const char * title){
    Serial.print(FORE_YELLOW);
    Serial.print(BGC_BLUE);
    Serial.print(title);
    Serial.print(BGC_BLACK);
    Serial.println(FORE_YELLOW);
}

void Logger::Error(const char * title){
    Serial.print(FORE_RED);
    Serial.print(BGC_BLUE);
    Serial.print(title);
    Serial.print(BGC_BLACK);
    Serial.println(FORE_RED); 
}

void Logger::Halt(const char* title){
    Serial.print(FORE_YELLOW);
    Serial.print(BGC_BLUE);
    Serial.print(title);
    Serial.print(BGC_BLACK);
    Serial.println(FORE_GREEN);
    Serial.print("Esp32 is halt");
    while (true){
        Serial.print("= ");
        delay(500);
    }
}