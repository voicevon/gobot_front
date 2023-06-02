#pragma once

#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

class WebConnfigurator_Parameter{
    public:
        WebConnfigurator_Parameter();
        // WebConnfigurator_Parameter(const char* parameter_name);
        void SetName(const char* parameter_name);
        void WriteToFile(AsyncWebParameter* p);
        bool IsMyName(const char * the_name);
        const char* GetName(){return &__parameter_name[0];};
        const char* readFile();
        /// @brief TODO: content will not be kept inside.
        void ReadFile_LongText(char* buffer){};
        
    private:
        const char* __GetSpiffsFilename();
        char __parameter_name[20];
        char __output_buffer[30];
        fs::FS* __fs;

};

