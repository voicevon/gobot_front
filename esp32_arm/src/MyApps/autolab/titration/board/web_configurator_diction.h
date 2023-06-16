#pragma once

#include "MyLibs/basic/memory_helper.h"
#include "MyLibs/utility/webserver_starter/base/ap_webserver_diction_base.h"
// #include "MyLibs/utility/webserver_starter/base/ap_webserver_diction_base.h"

#include "von/cpp/basic/c_string/fs_filename.h"
#define WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT 5


class WebConfiguratorDiction_Tiration:public ApWebserver_DictionBase{
	public:
		WebConfiguratorDiction_Tiration(){};
		void Init();
		/// @brief Add your parameters here
		Html_Parameter para_demo;
	
	private:
		Html_Parameter* __parameters[WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT];
		// FsFilename __html_filename;
};