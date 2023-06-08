#pragma once

#include "MyLibs/basic/memory_helper.h"
#include "MyLibs/utility/webserver_starter/base/ap_webserver_diction_base.h"
#include "MyLibs/basic/c_string/fs_filename.h"
#define WEB_CONFIGURATOR_PARAMETERS_COUNT_SERIAL_PORT_SNIFFER 13   // 10+3


class WebConfiguratorDiction_SerialPortSniffer:public WebConfigurator_DictionBase{
	public:
		WebConfiguratorDiction_SerialPortSniffer(){};
		void Init();
		/// @brief Add your parameters here
		// WebConnfigurator_Parameter para_device_id;
		WebConnfigurator_Parameter para_baud_rate;
		WebConnfigurator_Parameter para_bit_length;
		WebConnfigurator_Parameter para_verification;
		// WebConnfigurator_Parameter para_lua_script;
	
	private:
		WebConnfigurator_Parameter* __parameters[WEB_CONFIGURATOR_PARAMETERS_COUNT_SERIAL_PORT_SNIFFER];
		FsFilename __html_filename;
};