#pragma once

#include "i2c_master.h"
#define TOUCH_PAD_CHANNELS_COUNT_IN_NODE 14


class TouchPad_Node{
    public:

        void Init(I2C_Master* i2c_master, uint8_t i2c_slave_address, bool is_installed);
        void Read_via_I2C();
        bool Review_RxBuffer();

        char __mqtt_topic_substring[30];
        bool __has_changed_channel;
        const char* GetName(int point_id);
        
    private:
        I2C_SlaveNode __i2c_slave_node;
        I2C_Master* __i2c_master;
        TouchPad_Channel __all_channels[TOUCH_PAD_CHANNELS_COUNT_IN_NODE];
        uint8_t __rx_buffer[TOUCH_PAD_CHANNELS_COUNT_IN_NODE];

};