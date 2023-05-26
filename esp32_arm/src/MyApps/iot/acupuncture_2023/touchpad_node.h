#pragma once

#include "MyLibs/board/peripheral/bus_i2c/i2c_slave_node_agent.h"
#include "touchpad_channel.h"
#include "WString.h"


#define TOUCH_PAD_CHANNELS_COUNT_IN_NODE 14

class TouchPad_Node: public I2C_SlaveNodeAgent{
    public:
        void Init( uint8_t i2c_slave_address, bool is_installed);
        void Process_RxBuffer();

        String GetNodeStateString();
        String GetSensorsState();
        uint8_t GetSensorValue(int channel_id){ return __all_channels[channel_id].GetSensorValue();};
        
    private:
        TouchSensor __all_channels[TOUCH_PAD_CHANNELS_COUNT_IN_NODE];
        uint8_t __rx_buffer[TOUCH_PAD_CHANNELS_COUNT_IN_NODE];

};