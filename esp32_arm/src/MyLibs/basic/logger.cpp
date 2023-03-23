#include "logger.h"

// void Logger::SetStyle_begin(EnumStyle style_name){
void Logger::SetStyle(EnumStyle style_name, EnumStyle_position style_position){

    // switch (style_name){
    // case Info:

    //     break;
    
    // default:
    //     break;
    // }

    if ((style_name == EnumStyle::Info) && (style_position == EnumStyle_position::Begin)){
        Serial.print(FORE_YELLOW);
        Serial.print(BGC_ORANGE);
        Serial.print("[Info] ");
    }else if ((style_name == EnumStyle::Info) && (style_position == EnumStyle_position::End)){
        Serial.print(BGC_BLACK);
        Serial.println(FORE_GREEN);
    }
}

int Logger::sid = 0;
void Logger::Info(const char* title){
    // TRACE();
    Logger::SetStyle(EnumStyle::Info, EnumStyle_position::Begin);
    Serial.print(title);
    Logger::SetStyle(EnumStyle::Info, EnumStyle_position::End);

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
    Serial.print(BGC_RED);
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
    Serial.print(FORE_DARK_GRAY);
    Serial.print(BGC_BLACK);
    Serial.print("[Halt]");
    Serial.print(title);
    Serial.print(BGC_ORANGE);
    // Serial.println(FORE_GREEN);
    // Serial.print("Esp32 is halt  ");
    // esp_random()
    while (true){
        Serial.print("\t I am playing my game.  So funny...   So boring... ");
        delay(2000);
    }
}

// template <typename anyPrintable>
// void Logger::Print(const char* var_name, anyPrintable value){
//     Serial.print(var_name);
//     Serial.print(value);
// }

void Logger::__print_sid(){
    Serial.print("("); 
    Serial.print(Logger::sid); 
    Serial.print(")"); 
    Serial.print("\t");
    Logger::sid++;

}
void Logger::Print(const char* var_name, bool value){
    __print_sid();
    Serial.print(var_name);
    Serial.print("=\t");
    Serial.println(value);
}

void Logger::Print(const char* var_name, float value){
    __print_sid();
    Serial.print(var_name);
    Serial.print("=\t");
    Serial.println(value);
}

void Logger::Print(const char* key, const char* value){
    __print_sid();
    Serial.print(key);
    Serial.print("=\t");
    Serial.println(value);
}

void Logger::Print(const char* var_name, double value){
    __print_sid();
    Serial.print(var_name);
    Serial.print("=\t");
    Serial.println(value);
}

void Logger::Print(const char* var_name, int32_t value){
    __print_sid();
    Serial.print(var_name);
    Serial.print(" =\t");
    Serial.println(value);
}
void Logger::Print(const char* var_name, int64_t value){
    __print_sid();
    Serial.print(var_name);
    Serial.print(" =\t");
    Serial.println(value);
}

void Logger::Print(const char* var_name, uint32_t value){
    __print_sid();
    Serial.print(var_name);
    Serial.print(" =\t");
    Serial.println(value);
}

void Logger::Print(const char* var_name, unsigned long value){
    __print_sid();
    Serial.print(var_name);
    Serial.print(" =\t");
    Serial.println(value);
}

void Logger::Print(const char* var_name, const char value){
    __print_sid();
    Serial.print(var_name);
    Serial.print(" =\t");
    Serial.println(char(value));
}
