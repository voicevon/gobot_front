#include "touchpad_node.h"
#include "MyLibs/basic/logger.h"

void TouchPad_Node::Init(I2C_Master * i2c_master, uint8_t i2c_slave_address, bool is_installed){
    __i2c_master = i2c_master;
    __i2c_slave_node.Init(is_installed, i2c_slave_address, TOUCH_PAD_CHANNELS_COUNT_IN_NODE, __rx_buffer);   //TODO: TOUCH_PAD_CHANNELS_COUNT_IN_NODE is var

    for(int i=0; i<TOUCH_PAD_CHANNELS_COUNT_IN_NODE; i++){
        __all_channels[i].Init(i, TouchPad_Channel::EnumState::TOUCHED_OFF);
    }
}

// const char* TouchPad_Node::GetName(int point_id){
//     String name = "u";
//     name.concat(this->__i2c_slave_node.GetAddress());
//     name.concat("p");
//     name.concat(point_id);
//     name.toCharArray(this->__mqtt_topic_substring, 30);
//     return this->__mqtt_topic_substring;
// }


void TouchPad_Node::Read_via_I2C(){
    if (__i2c_slave_node.IsOnline())
        __i2c_master->ReadSlaveNode(&__i2c_slave_node);
}

bool TouchPad_Node::Review_RxBuffer(){
    uint8_t byte_index = 0;
    uint8_t bit_index;
    uint8_t* rx_buffer = __i2c_slave_node.GetRxBuffer();
    __has_changed_channel = false;
    // Logger::Debug("TouchPad_Node::Review_RxBuffer()");
    for (int i=0; i<TOUCH_PAD_CHANNELS_COUNT_IN_NODE; i++){
        // update state.
        if (i >=8) byte_index = 1;
        bit_index = i % 8;
        if (rx_buffer[byte_index] & (1 << bit_index)) {
        }else{
            __all_channels[i].SetStateToDied();
        }
        // review sensor's value
        __all_channels[i].Push_to_HistoryValues(*(rx_buffer + i));
        __has_changed_channel |=  __all_channels[i].Review_Sensor_Value_Whether_Changed();
        // Logger::Print("__has_changed_channel", __has_changed_channel);
    }
    return __has_changed_channel;
}

String TouchPad_Node::GetMqttPayloadString(){
    if(__i2c_slave_node.GetState() == I2C_SlaveNode::EnumState::NOT_INSTALLED) return String("I");
    if(__i2c_slave_node.GetState() == I2C_SlaveNode::EnumState::OFFLINE_DIED) return String("D");
    if(__i2c_slave_node.GetState() == I2C_SlaveNode::EnumState::ONLINE_CONNECTED) return String("C");
}

String TouchPad_Node::GetChannelsPayloadString(){
    String str = "";
    TouchPad_Channel* channel;
    for (int i=0; i<TOUCH_PAD_CHANNELS_COUNT_IN_NODE; i++){
        channel = &__all_channels[i];
        str.concat(channel->GetPayloadString());
    }
    return str;
    // return String("12345678901234");

}

