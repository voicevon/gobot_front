#include "web_configurator_diction_demo.h"
#include "MyLibs/basic/c_string/fs_filename.h"

void WebConfiguratorDiction_Demo::Init(){

    _Init(&__parameters[0], WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT);
    // any sequence. 
    _AppendParameter(&para_demo, "demo");

    HtmlFilename_of_Configurator = &__html_filename;
}