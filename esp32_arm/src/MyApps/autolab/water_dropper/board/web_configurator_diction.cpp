#include "web_configurator_diction.h"
#define PIN_GPIO_OF_CONFIG_BUTTON  -1

void WaterDropper_WebConfiguratorDiction::Init(){
    _Init(__parameters, WaterDropper_WEB_CONFIGURATOR_PARAMETERS_COUNT);
    // any sequence. 
    __html_filename.CopyFrom("/water_dropper.html");
    _AppendParameter(&para_timer, "timer");


    Gpio_of_ConfigButton  = PIN_GPIO_OF_CONFIG_BUTTON;
    HtmlFilename_of_Configurator = &__html_filename;
    
    
}