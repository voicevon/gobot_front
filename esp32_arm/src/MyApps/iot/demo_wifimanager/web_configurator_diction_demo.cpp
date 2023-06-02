#include "web_configurator_diction_demo.h"

void WebConfiguratorDiction_Demo::Init(){
    // any sequence. 
    __parameters[0] = &para_wifi_ssid;
    __parameters[1] = &para_wifi_pass;
    __parameters[2] = &para_admin_uid;
    __parameters[3] = &para_admin_password;

    para_demo.SetName("demo");
    __parameters[4] = &para_demo;

    Gpio_of_ConfigButton  = -1;
    HtmlFilename_of_Configurator = __html_file;
    
    // _parameters = __parameters[0];
    _Init(&__parameters[0], WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT);
    
}