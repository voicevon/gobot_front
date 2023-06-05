#include "board.h"

#define PIN_SOFT_SERIAL_MASTER_TX 20
#define PIN_SOFT_SERIAL_MASTER_RX 21
#define PIN_SOFT_SERIAL_SLAVE_TX 22
#define PIN_SOFT_SERIAL_SLAVE_RX 23

void SerialPortSniffer_Board::__InitHardware(){
    // init gpio.
    // pinMode(PIN_STATE_LED, OUTPUT);
    __rx_led.Init(0, PIN_STATE_LED, HIGH);
    master_serial.begin(115200, SWSERIAL_8N1, PIN_SOFT_SERIAL_MASTER_RX, PIN_SOFT_SERIAL_MASTER_TX, false);
    slave_serial.begin(115200, SWSERIAL_8N1, PIN_SOFT_SERIAL_SLAVE_RX, PIN_SOFT_SERIAL_SLAVE_TX, false);

}

void SerialPortSniffer_Board::Init(const char* app_welcome_statement){
    _InitSerial("Hi Xuming, I am Serial-Port-Sniffer board. Good luck......");
    Serial.print(COLOR_RESET);
    this->__InitHardware();
    this->_Init_SPIFFS();
    this->RepportRamUsage();
}

