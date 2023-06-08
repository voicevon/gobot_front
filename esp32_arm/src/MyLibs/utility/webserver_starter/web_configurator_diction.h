#pragma once

#include "MyLibs/basic/memory_helper.h"
#include "MyLibs/utility/webserver_starter/base/ap_webserver_diction_base.h"
#include "MyLibs/basic/c_string/fs_filename.h"
#define WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT 9   
#define PIN_GPIO_OF_CONFIG_BUTTON_AT_STANDARD 21


class WebConfiguratorDiction_Standard:public ApWebserver_DictionBase{
	public:
		WebConfiguratorDiction_Standard(){};
		void Init(){
			_Init(&__parameters[0], WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT);
		    Gpio_of_ConfigButton  = PIN_GPIO_OF_CONFIG_BUTTON_AT_STANDARD;
    		HtmlFilename_of_Configurator = &__html_file;
		}
	
	private:
		Html_Parameter* __parameters[WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT];
		// const char* __html_file="/serial_port_sniffer.html";
		FsFilename __html_filename;
};