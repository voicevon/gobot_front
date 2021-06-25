#pragma once

#include <Wire.h>



class Mcp23018{
    public:
        static Mcp23018& getInstance()
        {
            static Mcp23018 instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }
        void EnableSingleCoil(int logic_coil_id, bool enable_it);
        void DisableAllCoils(void);
    private:
        Mcp23018();
        // Mcp23018(Mcp23018 const& copy);            // Not Implemented
        // Mcp23018& operator=(Mcp23018 const& copy); // Not Implemented

        int __I2cAddress[8];   //Index is logic coil id, value is phical coil id
        int __PhysicalId[55];
        uint8_t __UINT8_ZERO = 0;
        uint8_t __CHIPS_COUNT;
        // uint8_t* __ZERO_POINTER;

};