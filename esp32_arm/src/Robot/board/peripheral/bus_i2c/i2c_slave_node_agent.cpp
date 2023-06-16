#include "i2c_slave_node_agent.h"
#include "von/cpp/utility/logger.h"



void I2C_SlaveNodeAgent::_Init(bool is_installed,uint8_t address, uint8_t rx_size){
    if (is_installed){
        __state = ONLINE_CONNECTED;
    }else{
        __state = NOT_INSTALLED;
    }
    __i2c_address = address;
    __rx_size = rx_size;

}




