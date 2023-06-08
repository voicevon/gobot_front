#pragma once
#include "MyLibs/utility/logger.h"
#include <ESPAsyncWebServer.h>
#include "html_parameter.h"
#include "MyLibs/basic/c_string/fs_filename.h"
#include "Robot/Sensor/position_binary_trigger/position_trigger.h"

// TODO:  base calss is Itemable.
class WebConfigurator_DictionBase{
	public:
        WebConnfigurator_Parameter* FindItem(const char* item_name);
		WebConnfigurator_Parameter para_wifi_ssid; 
		WebConnfigurator_Parameter para_wifi_pass; 	
        WebConnfigurator_Parameter para_admin_uid; 
		WebConnfigurator_Parameter para_admin_password;  

		WebConnfigurator_Parameter para_mqtt_broker;  
		WebConnfigurator_Parameter para_mqtt_port;  
		WebConnfigurator_Parameter para_mqtt_uid;  
		WebConnfigurator_Parameter para_mqtt_password;  
        
		WebConnfigurator_Parameter para_device_sid;  
		WebConnfigurator_Parameter para_lua_main;  

        FsFilename* HtmlFilename_of_Configurator;
        PositionTrigger* ConfigButton = nullptr;

    protected:
        WebConfigurator_DictionBase(){};  // To prevent be instanced.
        void _Init(WebConnfigurator_Parameter** first_para_pointer, int paras_count);
        void _AppendParameter(WebConnfigurator_Parameter* the_parameter, const char* para_name);
	private:
        WebConnfigurator_Parameter** __parameters;  // ** ？
        int __parameters_count;
        int __parameter_appending_index;
};
