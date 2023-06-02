#pragma once

#include "MyLibs/basic/memory_helper.h"
#include "MyLibs/utility/web_configuator/base/web_configurator_diction_base.h"
#define WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT 9   //8+1


class WebConfiguratorDiction_Demo:public WebConfigurator_DictionBase{
	public:
		WebConfiguratorDiction_Demo(){};
		void Init();

		// Add your parameters here
		WebConnfigurator_Parameter para_demo;
	
	private:
		WebConnfigurator_Parameter* __parameters[WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT];
		const char* __html_file="/config_demo.html";
};