#pragma once
#include "stdint.h"
#include "touchpad_channel.h"

class I2C_SlaveNode{
    public:
        void Init(uint8_t address, uint8_t rx_size, uint8_t* rx_buffer);
        uint8_t GetAddress(){return __i2c_address;};
        uint8_t GetSize() {return __rx_size;};
        uint8_t* GetRxBuffer(){return __rx_buffer;};
        const char* GetMqttPayload(int bit_index);

        bool IsForceOnline = false;
        const bool& IsOnline = _is_online;
        void SetOffline(){this->_is_online = false;};
        void SetOnline(){this->_is_online= true;};

        
    private:
        uint8_t __i2c_address;
        uint8_t* __rx_buffer;  // Why is pointer?  because rx_size is an unkown number to serve variable requirement.
        uint8_t __rx_size;
        bool _is_online = true;



};