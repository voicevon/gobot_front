#include "web_configurator_diction.h"

void WebConfiguratorDiction_Tiration::Init(){
    // any sequence. 
    __parameters[0] = &para_wifi_ssid;
    __parameters[1] = &para_wifi_pass;
    __parameters[2] = &para_admin_uid;
    __parameters[3] = &para_admin_password;

    para_demo.SetName("demo");
    __parameters[4] = &para_demo;

    // __html_filename.CopyFrom("/config_demo.html");
    // HtmlFilename_of_Configurator = &__html_filename;
    HtmlFilename_of_Configurator.CopyFrom("/config_demo.html");
    _Init(&__parameters[0], WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT);
    
}