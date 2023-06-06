#include "web_configurator_diction.h"
#define PIN_GPIO_OF_CONFIG_BUTTON_AT_SERIAL_PORT_SNIFFER -1

void WaterDropper_WebConfiguratorDiction::Init(){
    _Init(__parameters, WEB_CONFIGURATOR_PARAMETERS_COUNT_SERIAL_PORT_SNIFFER);
    // any sequence. 
    _AppendParameter(&para_device_id,"device_id");
    _AppendParameter(&para_baud_rate, "baud_rate");
    _AppendParameter(&para_bit_length, "bit_length");
    _AppendParameter(&para_verification, "verification");
    _AppendParameter(&para_lua_script, "lua");

    Gpio_of_ConfigButton  = PIN_GPIO_OF_CONFIG_BUTTON_AT_SERIAL_PORT_SNIFFER;
    HtmlFilename_of_Configurator = __html_file;
    
    // _parameters = __parameters[0];
    
}