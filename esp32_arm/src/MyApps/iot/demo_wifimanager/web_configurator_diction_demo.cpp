#include "web_configurator_diction_demo.h"

WebConfiguratorDiction_Demo::WebConfiguratorDiction_Demo(){
    // any sequence.
    __parameters[0] = &para_wifi_ssid;
    __parameters[1] = &para_wifi_pass;
    __parameters[2] = &para_admin_uid;
    __parameters[3] = &para_admin_password;
    __parameters[4] = &para_demo;
    
    _parameters = __parameters[0];
}