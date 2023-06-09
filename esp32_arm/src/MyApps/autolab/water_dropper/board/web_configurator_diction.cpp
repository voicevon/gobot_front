#include "web_configurator_diction.h"

void WaterDropper_WebConfiguratorDiction::Init(PositionTrigger* config_button){
    _Init(__parameters, WaterDropper_WEB_CONFIGURATOR_PARAMETERS_COUNT);
    _AppendParameter(&para_timer, "timer");
    
    ConfigButton = config_button;
    // HtmlFilename_of_Configurator = &__html_filename;
    // __html_filename.CopyFrom("/water_dropper.html");
    HtmlFilename_of_Configurator.CopyFrom("/water_dropper.html");
    
    
}