#pragma once
#include "Robot/board/board_base.h"
#include "Robot/board/display/mono_led_gpio.h"
// #include "../lua/uart.h"

// #define __serial_master Serial1
// #define __serial_slave Serial2

class SerialPortSniffer_Board: public BoardBase{
    public:
        void Init(const char* app_welcome_statement) override;
        // Mono_Led_GPIO* GetStateLed(){return &__rx_led;};

        void TestLeds();
        void TestSerialPortMaster();
        void TestSerialPortSlave();
        
        
    private:
        void __InitHardware();
        Mono_Led_GPIO __led_power;
        Mono_Led_GPIO __led_data_in;
        Mono_Led_GPIO __led_data_out;
        Mono_Led_GPIO __led_rf;
        // SoftwareSerial mast_serial;
        // EspSoftwareSerial::UART __serial_master;
        // EspSoftwareSerial::UART __serial_slave;



};

extern SerialPortSniffer_Board serial_port_sniffer_board;