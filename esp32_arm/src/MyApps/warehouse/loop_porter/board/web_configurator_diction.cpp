#include "web_configurator_diction.h"
#include "MyLibs/basic/c_string/fs_filename.h"
// #define PIN_GPIO_OF_CONFIG_BUTTON_AT_SERIAL_PORT_SNIFFER -1

void WebConfiguratorDiction_LoogPorter::Init(){
    _Init(&__parameters[0], WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT);
    // any sequence. 

    // Gpio_of_ConfigButton  = PIN_GPIO_OF_CONFIG_BUTTON_AT_SERIAL_PORT_SNIFFER;
    HtmlFilename_of_Configurator = &__html_filename;
    
    // _parameters = __parameters[0];
    
}