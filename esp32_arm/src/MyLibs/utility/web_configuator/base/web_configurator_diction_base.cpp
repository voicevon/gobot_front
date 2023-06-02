#include "web_configurator_diction_base.h"



void WebConfigurator_DictionBase::_Init(WebConnfigurator_Parameter* first_para, int paras_count){
    _parameters = first_para;
    _parameters_count = paras_count;
}

WebConnfigurator_Parameter* WebConfigurator_DictionBase::FindItem(const char* item_name){
    WebConnfigurator_Parameter* item;
    for(int i=0; i<_parameters_count; i++){
        item = &_parameters[i];
        if (item->IsMyName(item_name)){
            return item;
        }
    }
    Logger::Error("WebConfigurator_DictionBase::FindItem()");
    Logger::Halt("");
}