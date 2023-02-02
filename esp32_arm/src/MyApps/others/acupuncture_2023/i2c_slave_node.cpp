#include "i2c_slave_node.h"
#include "MyLibs/basic/logger.h"



void I2C_SlaveNode::Init(uint8_t address, uint8_t rx_size, uint8_t* rx_buffer){
    __i2c_address = address;
    __rx_size = rx_size;
    __rx_buffer = rx_buffer;

}


const char* I2C_SlaveNode::GetMqttPayload(int bit_index){
    int byte_index = 0;
    if (bit_index >= 8){
        byte_index++;
        bit_index -= 8;
    }
    // if (CurrentFlags[byte_index] & (1<<bit_index))
    //     return "ON";
    return "OFF";
}


