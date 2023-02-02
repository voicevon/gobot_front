#include "touchpad_node.h"


// void TouchPad_Node::Init(I2C_SlaveNode * i2c_slave_node){
void TouchPad_Node::Init(I2C_Master * i2c_master){
    __i2c_master = i2c_master;
    // for(int i=0; i<14; i++){
    //     if (i<installed_channel_count){
    //         __all_channels[i].Init(cell_id, i, TouchPad_Channel::EnumState::WROKING);
    //     }else{
    //         __all_channels->Init(cell_id, i, TouchPad_Channel::EnumState::NOT_INSTALLED);
    //     }
    // }
}

const char* TouchPad_Node::GetName(int point_id){
    String name = "u";
    name.concat(this->__i2c_slave_node.I2C_Address);
    name.concat("p");
    name.concat(point_id);
    name.toCharArray(this->__mqtt_topic_substring, 30);
    return this->__mqtt_topic_substring;
}


void TouchPad_Node::Read_via_I2C(){

}

bool TouchPad_Node::Review_RxBuffer(){
    uint8_t byte_index = 0;
    uint8_t bit_index;
    uint8_t* rx_buffer = __i2c_slave_node.GetRxBuffer();
    __has_changed_channel = false;
    for (int i=0; i<14; i++){
        // update state.
        if (i >=8) byte_index = 1;
        bit_index = i % 8;
        if (rx_buffer[byte_index] & (1 << bit_index)) {
            __all_channels[i].SetStateTo(TouchPad_Channel::EnumState::WROKING);
        }else{
            __all_channels[i].SetStateTo(TouchPad_Channel::EnumState::CHANNEL_DIED);
        }
        // review sensor's value
        __all_channels[i].Push_to_HistoryValues(*(rx_buffer + i));
        __has_changed_channel |=  __all_channels[i].Review_Sensor_Value_Whether_Changed();
    }
}