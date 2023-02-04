#pragma once

#include "MyLibs/board/peripheral/bus_i2c/i2c_iam_master.h"
#include "touchpad_channel.h"
#include "WString.h"


#define TOUCH_PAD_CHANNELS_COUNT_IN_NODE 14


class TouchPad_Node{
    public:

        void Init(I2C_IamMaster* i2c_master, uint8_t i2c_slave_address, bool is_installed);
        void Read_via_I2C();
        void Process_RxBuffer();

        String GetMqttPayloadString();
        String GetChannelsPayloadString();
        I2C_SlaveNodeAgent* Get_I2CSlaveNode(){return &__i2c_slave_node;};
        uint8_t GetSensorValue(int channel_id){ return __all_channels[channel_id].GetSensorValue();};

        // bool __has_changed_channel;  //??
        
    private:
        I2C_SlaveNodeAgent __i2c_slave_node;
        I2C_IamMaster* __i2c_master;
        TouchPad_Channel __all_channels[TOUCH_PAD_CHANNELS_COUNT_IN_NODE];
        uint8_t __rx_buffer[TOUCH_PAD_CHANNELS_COUNT_IN_NODE];

};