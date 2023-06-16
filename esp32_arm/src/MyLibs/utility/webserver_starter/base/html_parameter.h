#pragma once

#include <ESPAsyncWebServer.h>
#include "LittleFS.h"
// #include "SPIFFS.h"
#include "von/cpp/basic/c_string/c_string_base.h"


// class Html_Parameter: public C_String_Base{
class Html_Parameter{
    public:
        Html_Parameter();
        void SetName(const char* parameter_name);
        void WriteToFile(AsyncWebParameter* p);
        bool IsMyName(const char * the_name);
        const char* GetName(){return &__parameter_name[0];};
        const char* readFile();
        C_String_Base* ReadFile();
        /// @brief TODO: content will not be kept inside.
        void ReadFile_LongText(char* buffer){};
        
    private:
        const char* __GetSpiffsFilename();
        char __parameter_name[20];
        char __output_buffer[30];
        C_String_Base __output_c_string;
        fs::FS* __fs;

};

