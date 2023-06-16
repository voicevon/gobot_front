#pragma once

#include "MyLibs/basic/memory_helper.h"
#include "MyLibs/utility/webserver_starter/base/ap_webserver_diction_base.h"
#include "von/cpp/basic/c_string/fs_filename.h"
#define WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT 9   //8+1


class WebConfiguratorDiction_Demo:public ApWebserver_DictionBase{
	public:
		WebConfiguratorDiction_Demo(){};
		void Init();

		// Add your parameters here
		Html_Parameter para_demo;
	
	private:
		Html_Parameter* __parameters[WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT];
		// const char* __html_file="/config_demo.html";
		FsFilename __html_filename;
};