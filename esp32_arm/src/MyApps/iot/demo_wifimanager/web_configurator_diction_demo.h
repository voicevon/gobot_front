#pragma once

#include "MyLibs/basic/memory_helper.h"
#include "web_configurator_diction_base.h"

class WebConfiguratorDiction_Demo:public WebConfigurator_DictionBase{
	public:
		WebConfiguratorDiction_Demo();
		bool HasItem(const char* item_name) override;
		
	private:
		WebConnfigurator_Parameter __ssid = WebConnfigurator_Parameter("ssid");
		WebConnfigurator_Parameter __pass = WebConnfigurator_Parameter("pass");
		WebConnfigurator_Parameter __admin_uid = WebConnfigurator_Parameter("admin_uid");
		WebConnfigurator_Parameter __admin_password = WebConnfigurator_Parameter("admin_password");
		WebConnfigurator_Parameter* config_items[4];
};