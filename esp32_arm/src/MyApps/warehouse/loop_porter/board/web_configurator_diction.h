#pragma once

#include "MyLibs/basic/memory_helper.h"
#include "MyLibs/utility/webserver_starter/base/ap_webserver_diction_base.h"
// #include "von/basic/c_string/fs_filename.h"

#define WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT 12   // 8+2


class WebConfiguratorDiction_LoogPorter:public ApWebserver_DictionBase{
	public:
		WebConfiguratorDiction_LoogPorter(){};
		void Init();
		/// @brief Add your parameters here
		Html_Parameter para_warehouse_id;
		Html_Parameter para_baud_rate;
		Html_Parameter para_bit_length;
		Html_Parameter para_verification;
	
	private:
		Html_Parameter* __parameters[WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT];
		// FsFilename __html_filename;
		// const char* __html_file="/serial_port_sniffer.html";
};