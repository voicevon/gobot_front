#pragma once
#include <Arduino.h>
#include <Wire.h>

#define COIL_COUNT 53

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

        uint8_t __I2cAddress[COIL_COUNT];   //Index is logic coil id, value is phical coil id
        uint8_t __PhysicalId[COIL_COUNT];
        uint8_t __NextCoilId[COIL_COUNT];
        uint8_t __UINT8_ZERO = 0;
        uint8_t __CHIPS_COUNT;
        // uint8_t* __ZERO_POINTER;

};