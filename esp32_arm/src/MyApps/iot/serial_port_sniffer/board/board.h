#pragma once
#include "Robot/board/board_base.h"
#include "Robot/board/display/mono_led_gpio.h"
#include <SoftwareSerial.h>

#define PIN_STATE_LED 2


class SerialPortSniffer_Board: public BoardBase{
    public:
        void Init(const char* app_welcome_statement) override;
        Mono_Led_GPIO* GetStateLed(){return &__rx_led;};
        
        
    private:
        void __InitHardware();
        Mono_Led_GPIO __rx_led;
        // SoftwareSerial mast_serial;
        EspSoftwareSerial::UART master_serial;
        EspSoftwareSerial::UART slave_serial;


};