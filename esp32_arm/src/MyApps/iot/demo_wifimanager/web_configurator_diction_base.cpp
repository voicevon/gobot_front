#include "web_configurator_diction_base.h"



void WebConfigurator_DictionBase::_Init(WebConnfigurator_Parameter* first_item, int items_count){
    _items = first_item;
    _items_count = items_count;
}

WebConnfigurator_Parameter* WebConfigurator_DictionBase::FindItem(const char* item_name){
    WebConnfigurator_Parameter* item;
    for(int i=0; i<_items_count; i++){
        item = &_items[i];
        if (item->IsMyName(item_name)){
            return item;
        }
    }
    Logger::Error("WebConfigurator_DictionBase::FindItem()");
    Logger::Halt("");
}