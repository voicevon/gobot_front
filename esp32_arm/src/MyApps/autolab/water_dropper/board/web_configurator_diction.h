#pragma once

#include "MyLibs/basic/memory_helper.h"
#include "MyLibs/utility/web_configuator/base/web_configurator_diction_base.h"
#include "MyLibs/basic/c_string/fs_filename.h"

#define WaterDropper_WEB_CONFIGURATOR_PARAMETERS_COUNT 14   // 9+5


class WaterDropper_WebConfiguratorDiction:public WebConfigurator_DictionBase{
	public:
		WaterDropper_WebConfiguratorDiction(){};
		void Init();
		/// @brief Add your parameters here
		WebConnfigurator_Parameter para_timer;
	
	private:
		WebConnfigurator_Parameter* __parameters[WaterDropper_WEB_CONFIGURATOR_PARAMETERS_COUNT];
		FsFilename __html_filename;
};