#include "web_configurator_diction_base.h"



void WebConfigurator_DictionBase::_Init(WebConnfigurator_Parameter** first_para_pointer, int paras_count){
    __parameters = first_para_pointer;
    __parameters_count = paras_count;
    __parameter_appending_index = 0;

    _AppendParameter(&para_wifi_ssid, "ssid");
    _AppendParameter(&para_wifi_pass, "pass");
    _AppendParameter(&para_admin_uid, "admin_uid");
    _AppendParameter(&para_admin_password, "admin_password");

    _AppendParameter(&para_mqtt_broker, "mqtt_broker");
    _AppendParameter(&para_mqtt_port, "mqtt_port");
    _AppendParameter(&para_mqtt_uid, "mqtt_uid");
    _AppendParameter(&para_mqtt_password, "mqtt_password");
    _AppendParameter(&para_device_sid, "device_sid");


    // _parameters[0] = &para_wifi_ssid;
    // _parameters[1] = &para_wifi_pass;
    // _parameters[2] = &para_admin_uid;
    // _parameters[3] = &para_admin_password;
    // _parameters[4] = &para_mqtt_broker;
    // _parameters[5] = &para_mqtt_port;
    // _parameters[6] = &para_mqtt_uid;
    // _parameters[7] = &para_mqtt_password;

}

void WebConfigurator_DictionBase::_AppendParameter(WebConnfigurator_Parameter* the_parameter, const char* para_name){
    the_parameter->SetName(para_name);
    __AppendParameter(the_parameter);
    if (__parameter_appending_index >= __parameters_count){
        Logger::Error("WebConfigurator_DictionBase::_AppendParameter()  over size");
        Logger::Halt("");
    }
}

void WebConfigurator_DictionBase::__AppendParameter(WebConnfigurator_Parameter* new_parameter){
    __parameter_appending_index++;
    if (__parameter_appending_index >= __parameters_count){
        Logger::Error("WebConfigurator_DictionBase::_AppendParameter()  over size");
        Logger::Halt("");
    }
}

WebConnfigurator_Parameter* WebConfigurator_DictionBase::FindItem(const char* parameter_name){
    WebConnfigurator_Parameter* para;
    for(int i=0; i<__parameters_count; i++){
        para = *(__parameters + i);
        Logger::Print("WebConfigurator_DictionBase::FindItem()", para->GetName());
        if (para->IsMyName(parameter_name)){
            return para;
        }
    }
    Logger::Error("WebConfigurator_DictionBase::FindItem()");
    Logger::Print("parameter_name", parameter_name);
    Logger::Halt("");
}