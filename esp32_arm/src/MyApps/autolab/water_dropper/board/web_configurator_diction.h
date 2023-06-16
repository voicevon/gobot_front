#pragma once

#include "MyLibs/basic/memory_helper.h"
#include "MyLibs/utility/webserver_starter/base/ap_webserver_diction_base.h"
// #include "von/cpp/basic/c_string/fs_filename.h"

#define WaterDropper_WEB_CONFIGURATOR_PARAMETERS_COUNT 12   // 11+1


class WaterDropper_WebConfiguratorDiction:public ApWebserver_DictionBase{
	public:
		WaterDropper_WebConfiguratorDiction(){};
		void Init(PositionTrigger* config_button);
		Html_Parameter para_timer;
	
	private:
		Html_Parameter* __parameters[WaterDropper_WEB_CONFIGURATOR_PARAMETERS_COUNT];
		// FsFilename __html_filename;
};

extern WaterDropper_WebConfiguratorDiction water_dropper_webconfigurator_diction;