#pragma once

#include "MyLibs/basic/memory_helper.h"
#include "MyLibs/utility/webserver_starter/base/ap_webserver_diction_base.h"
#include "MyLibs/basic/c_string/fs_filename.h"
#define WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT 12   // 8+2


class WebConfiguratorDiction_LoogPorter:public WebConfigurator_DictionBase{
	public:
		WebConfiguratorDiction_LoogPorter(){};
		void Init();
		/// @brief Add your parameters here
		WebConnfigurator_Parameter para_warehouse_id;
		WebConnfigurator_Parameter para_baud_rate;
		WebConnfigurator_Parameter para_bit_length;
		WebConnfigurator_Parameter para_verification;
	
	private:
		WebConnfigurator_Parameter* __parameters[WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT];
		FsFilename __html_filename;
		// const char* __html_file="/serial_port_sniffer.html";
};