#pragma once

#include "MyLibs/basic/memory_helper.h"
#include "MyLibs/utility/web_configuator/base/web_configurator_diction_base.h"
#include "MyLibs/basic/c_string/fs_filename.h"
#define WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT 5


class WebConfiguratorDiction_Tiration:public WebConfigurator_DictionBase{
	public:
		WebConfiguratorDiction_Tiration(){};
		void Init();
		/// @brief Add your parameters here
		WebConnfigurator_Parameter para_demo;
	
	private:
		WebConnfigurator_Parameter* __parameters[WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT];
		FsFilename __html_filename;
};