#include "web_configurator_diction.h"

void WaterDropper_WebConfiguratorDiction::Init(PositionTrigger* config_button){
    _Init(__parameters, WaterDropper_WEB_CONFIGURATOR_PARAMETERS_COUNT);
    // any sequence. 
    __html_filename.CopyFrom("/water_dropper.html");
    _AppendParameter(&para_timer, "timer");
    
    ConfigButton = config_button;
    HtmlFilename_of_Configurator = &__html_filename;
    
    
}