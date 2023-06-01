#pragma once

#include "MyLibs/basic/memory_helper.h"
#include "web_configurator_diction_base.h"

class WebConfiguratorDiction_Demo:public WebConfigurator_DictionBase{
	public:
		WebConfiguratorDiction_Demo();
		// bool HasItem(const char* item_name) override;
		WebConnfigurator_Parameter test_para;
	
	private:
		WebConnfigurator_Parameter* config_items[5];
};