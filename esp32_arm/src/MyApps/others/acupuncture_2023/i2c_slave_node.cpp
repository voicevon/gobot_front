#include "i2c_slave_node.h"
#include "MyLibs/basic/logger.h"



void I2C_SlaveNode::Init(bool is_installed,uint8_t address, uint8_t rx_size, uint8_t* rx_buffer){
    if (is_installed){
        __state = ONLINE_CONNECTED;
    }else{
        __state = NOT_INSTALLED;
    }
    __i2c_address = address;
    __rx_size = rx_size;
    __rx_buffer = rx_buffer;

}




