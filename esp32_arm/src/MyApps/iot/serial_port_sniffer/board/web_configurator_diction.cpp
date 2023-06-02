#include "web_configurator_diction.h"
#define PIN_GPIO_OF_CONFIG_BUTTON_AT_SERIAL_PORT_SNIFFER -1

void WebConfiguratorDiction_SerialPortSniffer::Init(){
    // any sequence. 
    __parameters[0] = &para_wifi_ssid;
    __parameters[1] = &para_wifi_pass;
    __parameters[2] = &para_admin_uid;
    __parameters[3] = &para_admin_password;

    para_device_id.SetName("device_id");
    para_baud_rate.SetName("baud_rate");
    para_bit_length.SetName("bit_length");
    para_verification.SetName("verification");
    __parameters[4] = &para_device_id;
    __parameters[5] = &para_baud_rate;
    __parameters[6] = &para_bit_length;
    __parameters[7] = &para_verification;

    Gpio_of_ConfigButton  = PIN_GPIO_OF_CONFIG_BUTTON_AT_SERIAL_PORT_SNIFFER;
    HtmlFilename_of_Configurator = __html_file;
    
    // _parameters = __parameters[0];
    _Init(&__parameters[0], WEB_CONFIGURATOR_DEMO_PARAMETERS_COUNT);
    
}