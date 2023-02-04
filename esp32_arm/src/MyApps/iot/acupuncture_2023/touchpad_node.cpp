#include "touchpad_node.h"
#include "MyLibs/basic/logger.h"

void TouchPad_Node::Init(I2C_IamMaster * i2c_master, uint8_t i2c_slave_address, bool is_installed){
    __i2c_master = i2c_master;
    __i2c_slave_node.Init(is_installed, i2c_slave_address, TOUCH_PAD_CHANNELS_COUNT_IN_NODE, __rx_buffer);   //TODO: TOUCH_PAD_CHANNELS_COUNT_IN_NODE is var

    for(int i=0; i<TOUCH_PAD_CHANNELS_COUNT_IN_NODE; i++){
        __all_channels[i].Init(i, TouchPad_Channel::EnumState::TOUCHED_OFF);
    }
}

void TouchPad_Node::Read_via_I2C(){
    if (__i2c_slave_node.IsOnline())
        __i2c_master->ReadSlaveNode(&__i2c_slave_node);
}

void TouchPad_Node::Process_RxBuffer(){
    uint8_t* rx_buffer = __i2c_slave_node.GetRxBuffer();
    // Logger::Debug("TouchPad_Node::Review_RxBuffer()");
    for (int i=0; i<TOUCH_PAD_CHANNELS_COUNT_IN_NODE; i++){
        // review sensor's value
         __all_channels[i].Review_Sensor_Value(*(rx_buffer+i));
        // Logger::Print("__has_changed_channel", __has_changed_channel);
    }
}

String TouchPad_Node::GetNodeStateString(){
    if(__i2c_slave_node.GetState() == I2C_SlaveNodeAgent::EnumState::NOT_INSTALLED) return String("I");
    if(__i2c_slave_node.GetState() == I2C_SlaveNodeAgent::EnumState::OFFLINE_DIED) return String("D");
    if(__i2c_slave_node.GetState() == I2C_SlaveNodeAgent::EnumState::ONLINE_CONNECTED) return String("C");
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

