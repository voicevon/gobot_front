#include "board.h"
#include "Robot/board/peripheral/uart_isr.h"


// //  Master has 5 wires
// //TODO:  rename to HARD
#define PIN_HARD_SERIAL_MASTER_TX 17
#define PIN_HARD_SERIAL_MASTER_RX 16

// Slave has 3 wires
#define PIN_HARD_SERIAL_SLAVE_TX 25
#define PIN_HARD_SERIAL_SLAVE_RX 33

// #define BLINK_GPIO GPIO_NUM_2

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

    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    init_master_uart_with_isr(UART_NUM_1, &uart_config, PIN_HARD_SERIAL_MASTER_RX, PIN_HARD_SERIAL_MASTER_TX);
    init_slave_uart_with_isr(UART_NUM_2, &uart_config, PIN_HARD_SERIAL_SLAVE_RX, PIN_HARD_SERIAL_SLAVE_TX);
}

void SerialPortSniffer_Board::TestSerialPortMaster(){
    for(char i= 32; i<99;i++){
        // __serial_master.write(i);
        // uart_write_bytes();
	    uart_write_bytes(UART_NUM_1, &i, 1);
        Logger::Print("Master sending", i);
        delay(100);
        // if (__serial_master.available()){
        //     char xx = __serial_master.read();
        //     Logger::Print("                 Master received",xx);
        // }

        if (master_rx_bytes_count >0){
            char xx = master_rx_buffer[0];
            Logger::Print("                 Master received",xx);
            master_rx_bytes_count--;
        }
        if (slave_rx_bytes_count >0){
            char xx = slave_rx_buffer[0];
            Logger::Print("                 Slave received",xx);
            slave_rx_bytes_count--;
        }
    }
}

void SerialPortSniffer_Board::TestSerialPortSlave(){
    for(char i= 32; i<99;i++){
        // __serial_slave.write(i);
	    uart_write_bytes(UART_NUM_2, &i, 1);
        Logger::Print("Slave sending", i);
        delay(100);
        // if (__serial_slave.available()){
        //     char xx = __serial_slave.read();
        //     Logger::Print("                 Slave received",xx);
        // }
        if (master_rx_bytes_count >0){
            char xx = master_rx_buffer[0];
            Logger::Print("                 Master received",xx);
            master_rx_bytes_count--;
        }
        if (slave_rx_bytes_count >0){
            char xx = slave_rx_buffer[0];
            Logger::Print("                 Slave received",xx);
            slave_rx_bytes_count--;
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