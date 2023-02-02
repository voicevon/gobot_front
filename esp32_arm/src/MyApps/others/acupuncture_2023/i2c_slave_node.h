#pragma once
#include "stdint.h"
#include "touchpad_channel.h"

//TODO: rename to I2c_SlaveAgent
class I2C_SlaveNode{
    public:
        uint8_t I2C_Address;
        void Init(uint8_t cell_id, uint8_t installed_channel_count);

        const char* GetMqttPayload(int bit_index);

        bool IsForceOnline = false;
        const bool& IsOnline = _is_online;
        void SetOffline(){this->_is_online = false;};
        void SetOnline(){this->_is_online= true;};

        uint8_t* GetRxBuffer(){return __rx_buffer;};
        
    private:
        uint8_t __rx_buffer[14];  // TODO:  become a pointer,  because 14 is an unkown number for variable requirement.
        bool _is_online = true;



};