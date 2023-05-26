#pragma once
#include "stdint.h"

class I2C_SlaveNodeAgent{
    public:
        enum EnumState{
            NOT_INSTALLED,
            OFFLINE_DIED,
            ONLINE_CONNECTED,
        };
        // uint8_t GetAddress(){return __i2c_address;};
        // uint8_t GetSize() {return __rx_size;};
        // EnumState GetState(){return __state;};
        uint8_t* GetRxBuffer(){return _rx_buffer;};
        // const uint8_t* &RxBuffer = *_rx_buffer;

        const uint8_t &Address = __i2c_address;
        const uint8_t &RxBufferSize = __rx_size;
        const EnumState &State = __state;

        bool IsOnline(){return __state==ONLINE_CONNECTED;};
        void SetOffline(){__state=OFFLINE_DIED;};
        void SetOnline(){__state = ONLINE_CONNECTED;};  //???  For test-jig , this is useful.

    protected:
        uint8_t* _rx_buffer;  // Why is pointer?  because rx_size is an unkown number to serve variable requirement.
        void _Init(bool is_installed, uint8_t address, uint8_t rx_size);
        uint8_t _Get_i2c_Addr(){return __i2c_address;};

    private:
        uint8_t __i2c_address;
        uint8_t __rx_size;
        EnumState __state;
};