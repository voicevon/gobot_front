#include "board.h"

//  Master has 5 wires
#define PIN_SOFT_SERIAL_MASTER_TX 17
#define PIN_SOFT_SERIAL_MASTER_RX 16

// Slave has 3 wires
#define PIN_SOFT_SERIAL_SLAVE_TX 25
#define PIN_SOFT_SERIAL_SLAVE_RX 33

// Led-3  is red
#define PIN_LED_RF 21
#define PIN_LED_DATA_OUT 19
#define PIN_LED_DATA_IN 18
#define PIN_LED_POWER 4

#define PIN_CONFIG_BUTTON 32


void SerialPortSniffer_Board::__InitHardware(){
    __led_power.Init(0, PIN_LED_POWER, LOW);
    __led_data_in.Init(1, PIN_LED_DATA_IN, LOW);
    __led_data_out.Init(2, PIN_LED_DATA_OUT, LOW);
    __led_rf.Init(3,PIN_LED_RF, LOW);

    // __serial_master.begin(115200, 134217756U ,PIN_SOFT_SERIAL_MASTER_RX, PIN_SOFT_SERIAL_MASTER_TX);
    // __serial_slave.begin(115200, 134217756U,PIN_SOFT_SERIAL_SLAVE_RX, PIN_SOFT_SERIAL_SLAVE_TX);
    

}

void SerialPortSniffer_Board::Init(const char* app_welcome_statement){
    _InitSerial("Hi Xuming, I am Serial-Port-Sniffer board. Good luck......");
    Serial.print(COLOR_RESET);
    this->__InitHardware();
    this->_Init_SPIFFS();
    this->RepportRamUsage();
}

void SerialPortSniffer_Board::TestSerialPortMaster(){
    for(char i= 32; i<99;i++){
        __serial_master.write(i);
        Logger::Print("Master sending", i);
        delay(100);
        if (__serial_master.available()){
            char xx = __serial_master.read();
            Logger::Print("                 Master received",xx);
        }
    }
}

void SerialPortSniffer_Board::TestSerialPortSlave(){
    for(char i= 32; i<99;i++){
        __serial_slave.write(i);
        Logger::Print("Slave sending", i);
        delay(100);
        if (__serial_slave.available()){
            char xx = __serial_slave.read();
            Logger::Print("                 Slave received",xx);
        }
    }
}

void SerialPortSniffer_Board::TestLeds(){
    for(int i=0; i<3; i++){
        __led_power.TurnOn();
        delay(1000);
        __led_data_in.TurnOn();
        delay(1000);
        __led_data_out.TurnOn();
        delay(1000);
        __led_rf.TurnOn();
        delay(1000);

        __led_power.TurnOff();
        __led_data_in.TurnOff();
        __led_data_out.TurnOff();
        __led_rf.TurnOff();
        delay(1000);
    }
}


SerialPortSniffer_Board serial_port_sniffer_board;