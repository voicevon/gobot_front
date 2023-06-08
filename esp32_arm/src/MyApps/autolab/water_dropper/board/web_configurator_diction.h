#pragma once

#include "MyLibs/basic/memory_helper.h"
#include "MyLibs/utility/webserver_starter/base/ap_webserver_diction_base.h"
#include "MyLibs/basic/c_string/fs_filename.h"

#define WaterDropper_WEB_CONFIGURATOR_PARAMETERS_COUNT 14   // 9+5


class WaterDropper_WebConfiguratorDiction:public ApWebserver_DictionBase{
	public:
		WaterDropper_WebConfiguratorDiction(){};
		void Init(PositionTrigger* config_button);
		Html_Parameter para_timer;
	
	private:
		Html_Parameter* __parameters[WaterDropper_WEB_CONFIGURATOR_PARAMETERS_COUNT];
		FsFilename __html_filename;
};