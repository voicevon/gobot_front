#include "MyLibs/basic/logger.h"
#include "web-configurator_parameter.h"

WebConnfigurator_Parameter::WebConnfigurator_Parameter(const char* item_name){
    for(int i=0; i<20; i++){
        char xx = *(item_name + i);
        __item_name[i] = xx;
        if (xx == 0x00){
            return;
        }
    }
}

const char* WebConnfigurator_Parameter::GetSpiffsFilename(){
    __output_buffer[0] = '/';
    for(int i=0; i< 30; i++){
        char xx = __item_name[i];
        __output_buffer[i+1] = xx;
        if (xx = 0x00){
            return &__output_buffer[0];
        }
    }
};

void WebConnfigurator_Parameter::WriteToFile(AsyncWebParameter* p){

}

// void WebConnfigurator_Parameter::Link_AsyncWebParameter(){

// }

bool WebConnfigurator_Parameter::IsMyName(const char * the_name){
    for(int i=0; i< 30; i++){
        char xx = *(the_name + i);
        if (xx != __item_name[i]){
            return false;
        }
        if (xx == 0x00){
            return true;
        }
    }
    Logger::Error("WebConnfigurator_Parameter::IsMyName()");
    Logger::Print("parameter", the_name);
    Logger::Halt("");
}
