#include "touchpad_node.h"
#include "MyLibs/basic/logger.h"

void TouchPad_Node::Init( uint8_t i2c_slave_address, bool is_installed){
    // __i2c_master = i2c_master;
    _Init(is_installed, i2c_slave_address, TOUCH_PAD_CHANNELS_COUNT_IN_NODE);   //TODO: TOUCH_PAD_CHANNELS_COUNT_IN_NODE is var

    for(int i=0; i<TOUCH_PAD_CHANNELS_COUNT_IN_NODE; i++){
        __all_channels[i].Init(i, TouchPad_Channel::EnumState::TOUCHED_OFF);
    }
    _rx_buffer = __rx_buffer;
}

void TouchPad_Node::Process_RxBuffer(){
    for (int i=0; i<TOUCH_PAD_CHANNELS_COUNT_IN_NODE; i++){
        // review sensor's value
        __all_channels[i].Review_Sensor_Value(*(_rx_buffer+i));
    }
}

String TouchPad_Node::GetNodeStateString(){
    if(State == EnumState::ONLINE_CONNECTED) return String("C");
    if(State == EnumState::OFFLINE_DIED) return String("D");
    // if(GetState() == I2C_SlaveNodeAgent::EnumState::NOT_INSTALLED)
    return String("I");

}

String TouchPad_Node::GetChannelsStateString(){
    String str = "";
    TouchPad_Channel* channel;
    for (int i=0; i<TOUCH_PAD_CHANNELS_COUNT_IN_NODE; i++){
        channel = &__all_channels[i];
        str.concat(channel->GetStateString());
    }
    return str;

}

