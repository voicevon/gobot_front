#pragma once
#include "MyLibs/utility/logger.h"
#include <ESPAsyncWebServer.h>
#include "web-configurator_parameter.h"

// TODO:  base calss is Itemable.
class WebConfigurator_DictionBase{
	public:
        WebConnfigurator_Parameter* FindItem(const char* item_name);
        const char* HtmlFilename_of_Configurator;
        int8_t Gpio_of_ConfigButton;
		WebConnfigurator_Parameter para_wifi_ssid; 
		WebConnfigurator_Parameter para_wifi_pass; 	
        WebConnfigurator_Parameter para_admin_uid; 
		WebConnfigurator_Parameter para_admin_password;  
		WebConnfigurator_Parameter para_mqtt_broker;  
		WebConnfigurator_Parameter para_mqtt_port;  
		WebConnfigurator_Parameter para_mqtt_uid;  
		WebConnfigurator_Parameter para_mqtt_password;  
		WebConnfigurator_Parameter para_device_sid;  


    protected:
        WebConfigurator_DictionBase(){Gpio_of_ConfigButton=-1; };  // To prevent be instanced.
        void _Init(WebConnfigurator_Parameter** first_para_pointer, int paras_count);
        void _AppendParameter(WebConnfigurator_Parameter* the_parameter, const char* para_name);
		
	private:
        WebConnfigurator_Parameter** __parameters;  // ** ？
        int __parameters_count;
        int __parameter_appending_index;
        void __AppendParameter(WebConnfigurator_Parameter* new_parameter);
};
