#pragma once

#include <ESPAsyncWebServer.h>

class WebConnfigurator_Parameter{
    public:
        WebConnfigurator_Parameter(const char* item_name);
        const char* GetName(){return &__item_name[0];};
        const char* GetSpiffsFilename();
        bool IsMyName(const char * the_name);
        // void Link_AsyncWebParameter(AsyncWebParameter* p);
        void WriteToFile(AsyncWebParameter* p);

    private:
        char __item_name[20];
        char __output_buffer[30];

};

