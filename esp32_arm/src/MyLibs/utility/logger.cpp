#include "logger.h"

void Logger::SetStyle(EnumStyle style_name, EnumStyle_position style_position){

    if (style_name == EnumStyle::Debug){
        if (style_position == EnumStyle_position::Title_Begin){
            Serial.print(FORE_YELLOW);
            Serial.print(BGC_GREEN);
            Serial.print("[Debug] ");
            return;
        }else if (style_position == EnumStyle_position::Window_Begin){
            Serial.print(BGC_BLACK);
            Serial.println(FORE_GREEN);
            return;
        }
    }

    if (style_name == EnumStyle::Info) {
        if (style_position == EnumStyle_position::Title_Begin){
            Serial.print(FORE_YELLOW);
            Serial.print(BGC_CYAN);
            Serial.print("[Info] ");
            return;
        }else if (style_position == EnumStyle_position::Window_Begin){
            Serial.print(BGC_BLACK);
            Serial.println(FORE_CYAN);
            return;
        }
    }


    if (style_name == EnumStyle::Warning){
        if (style_position == EnumStyle_position::Title_Begin){
            Serial.print(FORE_BLUE);
            Serial.print(BGC_YELLLOW);
            Serial.print("[Warn] ");
            return;
        }else if (style_position == EnumStyle_position::Window_Begin){
            Serial.print(BGC_BLACK);
            Serial.println(FORE_YELLOW);
            return;
        }
    }
    if (style_name == EnumStyle::Error){
        if (style_position == EnumStyle_position::Title_Begin){
            Serial.print(FORE_BLUE);
            Serial.print(BGC_RED);
            Serial.print("[Error] ");
            return;
        }else if (style_position == EnumStyle_position::Window_Begin){
            Serial.print(BGC_BLACK);
            Serial.println(FORE_RED);
            return;
        }
    }    
}

int Logger::sid = 0;

void Logger::Info(const char* title){
    // TRACE();
    Logger::SetStyle(EnumStyle::Info, EnumStyle_position::Title_Begin);
    Serial.print(title);
    Logger::SetStyle(EnumStyle::Info, EnumStyle_position::Window_Begin);

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
    Logger::SetStyle(EnumStyle::Debug, EnumStyle_position::Title_Begin);
    Serial.print(title);
    Logger::SetStyle(EnumStyle::Debug, EnumStyle_position::Window_Begin);
    // Serial.print(FORE_YELLOW);
    // Serial.print(BGC_BLUE);
    // Serial.print(title);
    // Serial.print(BGC_BLACK);
    // Serial.println(FORE_PINK);
}

void Logger::Warn(const char * title){
    Logger::SetStyle(EnumStyle::Warning, EnumStyle_position::Title_Begin);
    Serial.print(title);
    Logger::SetStyle(EnumStyle::Warning, EnumStyle_position::Window_Begin);
}

void Logger::Error(const char * title){
    Logger::SetStyle(EnumStyle::Error, EnumStyle_position::Title_Begin);
    Serial.print(title);
    Logger::SetStyle(EnumStyle::Error, EnumStyle_position::Window_Begin);
}

void Logger::Halt(const char* title){
    Serial.print(FORE_DARK_GRAY);
    Serial.print(BGC_BLACK);
    Serial.print("[Halt]");
    Serial.print(title);
    Serial.print(BGC_YELLLOW);
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

void Logger::Print(const char* var_name, const bool value){
    __print_sid();
    Serial.print(var_name);
    Serial.print("=\t");
    Serial.println(value);
}

void Logger::Print(const char* var_name, const float value){
    __print_sid();
    Serial.print(var_name);
    Serial.print("=\t");
    Serial.println(value);
}

void Logger::Print(const char* var_name, const double value){
    __print_sid();
    Serial.print(var_name);
    Serial.print("=\t");
    Serial.println(value);
}

void Logger::Print(const char* var_name, const int32_t value){
    __print_sid();
    Serial.print(var_name);
    Serial.print(" =\t");
    Serial.println(value);
}

void Logger::Print(const char* var_name, const int64_t value){
    __print_sid();
    Serial.print(var_name);
    Serial.print(" =\t");
    Serial.println(value);
}

void Logger::Print(const char* var_name, const uint32_t value){
    __print_sid();
    Serial.print(var_name);
    Serial.print(" =\t");
    Serial.println(value);
}

void Logger::Print(const char* var_name, const unsigned long value){
    __print_sid();
    Serial.print(var_name);
    Serial.print(" =\t");
    Serial.println(value);
}
void Logger::Print(const char* var_name, const long value){
    __print_sid();
    Serial.print(var_name);
    Serial.print(" =\t");
    Serial.println(value);
}


void Logger::Print(const char* var_name, const char single_char){
    __print_sid();
    Serial.print(var_name);
    Serial.print(" =\t");
    Serial.println(single_char);
}

void Logger::Print(const char* key, const char* chars){
    __print_sid();
    Serial.print(key);
    Serial.print("=\t");
    Serial.println(chars);   // will print a string.
    // Serial.println(*chars);  // will print single char.

}
// void Logger::Print(const char* key, const char*& chars){
//     __print_sid();
//     Serial.print(key);
//     Serial.print("=\t");
//     Serial.println(chars);   // will print a string.
//     // Serial.println(*chars);  // will print single char.

// }

// void Logger::Print(const char* var_name, const char chars[]){
//     __print_sid();
//     Serial.print(key);
//     Serial.print("=\t");
//     Serial.println(chars);
// }

