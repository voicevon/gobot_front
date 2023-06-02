#include "web_configurator_diction_base.h"



void WebConfigurator_DictionBase::_Init(WebConnfigurator_Parameter** first_para_pointer, int paras_count){
    para_wifi_ssid.SetName("ssid");
    para_wifi_pass.SetName("pass");
    para_admin_uid.SetName("admin_uid");
    para_admin_password.SetName("admin_password");
    _parameters = first_para_pointer;
    _parameters_count = paras_count;

}

WebConnfigurator_Parameter* WebConfigurator_DictionBase::FindItem(const char* parameter_name){
    WebConnfigurator_Parameter* para;
    for(int i=0; i<_parameters_count; i++){
        para = *(_parameters + i);
        Logger::Print("WebConfigurator_DictionBase::FindItem()", para->GetName());
        if (para->IsMyName(parameter_name)){
            return para;
        }
    }
    Logger::Error("WebConfigurator_DictionBase::FindItem()");
    Logger::Print("parameter_name", parameter_name);
    Logger::Halt("");
}