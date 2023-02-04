#pragma once
#include "stdint.h"
// #include "touchpad_channel.h"

class I2C_SlaveNodeAgent{
    public:
        enum EnumState{
            NOT_INSTALLED,
            OFFLINE_DIED,
            ONLINE_CONNECTED,
        };
        void Init(bool is_installed, uint8_t address, uint8_t rx_size, uint8_t* rx_buffer);
        uint8_t GetAddress(){return __i2c_address;};
        uint8_t GetSize() {return __rx_size;};
        uint8_t* GetRxBuffer(){return __rx_buffer;};
        EnumState GetState(){return __state;};
        
        // bool IsForceOnline = false;   //???
        bool IsOnline(){return __state==ONLINE_CONNECTED;};
        void SetOffline(){__state=OFFLINE_DIED;};
        void SetOnline(){__state = ONLINE_CONNECTED;};  //???  For test-jig , this is useful.


        
    private:
        uint8_t __i2c_address;
        uint8_t* __rx_buffer;  // Why is pointer?  because rx_size is an unkown number to serve variable requirement.
        uint8_t __rx_size;
        EnumState __state;
};