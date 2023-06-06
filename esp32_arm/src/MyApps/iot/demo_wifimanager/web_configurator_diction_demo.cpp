#include "web_configurator_diction_demo.h"
#include "MyLibs/basic/c_string/fs_filename.h"

void WebConfiguratorDiction_Demo::Init(){
    // any sequence. 

    _Init(&__parameters[0], WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT);
    _AppendParameter(&para_demo, "demo");

    Gpio_of_ConfigButton  = -1;
    HtmlFilename_of_Configurator = &__html_filename;
}