#pragma once
#include "MyLibs/utility/logger.h"
#include <ESPAsyncWebServer.h>
#include "web-configurator_parameter.h"


class WebConfigurator_DictionBase{
	public:
        WebConnfigurator_Parameter* FindItem(const char* item_name);
        const char* HtmlFilename_of_Configurator;
        int8_t Gpio_of_ConfigButton;
		WebConnfigurator_Parameter para_wifi_ssid; //= WebConnfigurator_Parameter("ssid");
		WebConnfigurator_Parameter para_wifi_pass; //= WebConnfigurator_Parameter("pass");	
        WebConnfigurator_Parameter para_admin_uid; //= WebConnfigurator_Parameter("admin_uid");
		WebConnfigurator_Parameter para_admin_password;  // = WebConnfigurator_Parameter("admin_password");

    protected:
        WebConfigurator_DictionBase(){Gpio_of_ConfigButton=-1; };  // To prevent be instanced.
        void _Init(WebConnfigurator_Parameter** first_para_pointer, int paras_count);



		
	private:
        WebConnfigurator_Parameter** _parameters;  // ** ？
        int _parameters_count;
};
