#pragma once
#include "MyLibs/utility/logger.h"
#include <ESPAsyncWebServer.h>
#include "html_parameter.h"
#include "MyLibs/basic/c_string/fs_filename.h"
#include "Robot/Sensor/position_binary_trigger/position_trigger.h"

// TODO:  base calss is Itemable.
class ApWebserver_DictionBase{
	public:
        Html_Parameter* FindItem(const char* item_name);
		Html_Parameter para_wifi_ssid; 
		Html_Parameter para_wifi_pass; 	
        Html_Parameter para_admin_uid; 
		Html_Parameter para_admin_password;  

		Html_Parameter para_mqtt_broker;  
		Html_Parameter para_mqtt_port;  
		Html_Parameter para_mqtt_uid;  
		Html_Parameter para_mqtt_password;  
        
		Html_Parameter para_device_sid;  
		Html_Parameter para_lua_main;  

        FsFilename* HtmlFilename_of_Configurator;
        PositionTrigger* ConfigButton = nullptr;

    protected:
        ApWebserver_DictionBase(){};  // To prevent be instanced.
        void _Init(Html_Parameter** first_para_pointer, int paras_count);
        void _AppendParameter(Html_Parameter* the_parameter, const char* para_name);
	private:
        Html_Parameter** __parameters;  // ** ？
        int __parameters_count;
        int __parameter_appending_index;
};
