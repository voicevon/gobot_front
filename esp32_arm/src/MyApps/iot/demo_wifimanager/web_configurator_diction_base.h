#pragma once
#include "MyLibs/basic/logger.h"
#include <ESPAsyncWebServer.h>
#include "web-configurator_parameter.h"


class WebConfigurator_DictionBase{
	public:
		// virtual bool HasItem(const char* item_name);
        WebConnfigurator_Parameter* FindItem(const char* item_name);
        const char* HtmlFilename_of_Configurator;
        int8_t Gpio_of_ConfigButton;
		WebConnfigurator_Parameter __ssid = WebConnfigurator_Parameter("ssid");
		WebConnfigurator_Parameter __pass = WebConnfigurator_Parameter("pass");	
        WebConnfigurator_Parameter __admin_uid = WebConnfigurator_Parameter("admin_uid");
		WebConnfigurator_Parameter __admin_password = WebConnfigurator_Parameter("admin_password");
    protected:
        WebConfigurator_DictionBase(){};
        void _Init(WebConnfigurator_Parameter* first_item, int items_count);
        WebConnfigurator_Parameter* _items;
        int _items_count;

		
	private:

};
