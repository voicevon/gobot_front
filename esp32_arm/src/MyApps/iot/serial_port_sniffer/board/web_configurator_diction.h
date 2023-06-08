#pragma once

#include "MyLibs/basic/memory_helper.h"
#include "MyLibs/utility/webserver_starter/base/ap_webserver_diction_base.h"
#include "MyLibs/basic/c_string/fs_filename.h"
#define WEB_CONFIGURATOR_PARAMETERS_COUNT_SERIAL_PORT_SNIFFER 15   // 11+3


class WebConfiguratorDiction_SerialPortSniffer:public ApWebserver_DictionBase{
	public:
		WebConfiguratorDiction_SerialPortSniffer(){};
		void Init();
		/// @brief Add your parameters here
		// Html_Parameter para_device_id;
		Html_Parameter para_baud_rate;
		Html_Parameter para_bit_length;
		Html_Parameter para_verification;
		// Html_Parameter para_lua_script;
	
	private:
		Html_Parameter* __parameters[WEB_CONFIGURATOR_PARAMETERS_COUNT_SERIAL_PORT_SNIFFER];
		FsFilename __html_filename;
};