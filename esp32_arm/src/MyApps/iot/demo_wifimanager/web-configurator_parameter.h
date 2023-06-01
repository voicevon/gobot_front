#pragma once

#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

class WebConnfigurator_Parameter{
    public:
        WebConnfigurator_Parameter();
        WebConnfigurator_Parameter(const char* item_name);
        void SetName(const char* item_name);
        void WriteToFile(AsyncWebParameter* p);
        bool IsMyName(const char * the_name);
        const char* GetName(){return &__item_name[0];};
        // const char* GetValue(){return &_item_value[0];};
        const char* readFile();
        /// @brief object will not keep the buffer
        void ReadFile_LongText(char* buffer){};
        
    private:
        const char* GetSpiffsFilename();
        void writeFile(fs::FS &fs, const char * message);
        char __item_name[20];
        // char __item_value[30];
        char __output_buffer[30];

};

