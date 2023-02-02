#pragma once
#include "stdint.h"
#include "touchpad_channel.h"

class I2C_SlaveNode{
    public:
        enum EnumState{
            NOT_INSTALLED,
            OFFLINE,
            ONLINE,
        };
        void Init(bool is_installed, uint8_t address, uint8_t rx_size, uint8_t* rx_buffer);
        uint8_t GetAddress(){return __i2c_address;};
        uint8_t GetSize() {return __rx_size;};
        uint8_t* GetRxBuffer(){return __rx_buffer;};
        // const char* GetMqttPayload(int bit_index);

        bool IsForceOnline = false;
        // const bool& IsOnline = _is_online;
        bool IsOnline(){return __state==ONLINE;};
        void SetOffline(){__state=OFFLINE;};
        // void SetOnline(){__state = ONLINE;};  //???  For test-jig , this is useful.

        
    private:
        uint8_t __i2c_address;
        uint8_t* __rx_buffer;  // Why is pointer?  because rx_size is an unkown number to serve variable requirement.
        uint8_t __rx_size;
        // bool _is_online = true;
        EnumState __state;



};