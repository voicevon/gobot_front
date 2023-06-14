#pragma once

#include <HardwareSerial.h>
#include <Wire.h>
#include <esp_task_wdt.h>

#include <Adafruit_MCP23X17.h>
#include <Adafruit_VL53L0X.h>
#include <Adafruit_APDS9960.h>
#include "MyLibs/utility/logger.h"

class BoardBase{
    public:
        virtual void Init(const char* app_welcome_statement);
        virtual float ReadBatteryVolt(){return 1.234f;};
        static uint8_t Assign_ledc_channel();
        
        void RepportRamUsage();  // TODO::  be static
        void EnableWatchdog(uint32_t WDT_TIMEOUT);

    protected:
        // TODO:: be static
        void _InitSerial(const char* board_statement);
        static uint8_t Assign_i2c_bus_id(){return __i2c_bus_index; __i2c_bus_index++;};
        // void ScanI2cBus(TwoWire* i2c_bus, const char* printing_topic);
        // bool _Begin_I2cBus(TwoWire* i2c_bus, uint8_t pin_sda, uint8_t pin_scl, uint32_t frequency);
        bool _Begin_Mcp23018(Adafruit_MCP23X17* mcp, uint8_t i2c_address, TwoWire* i2c_bus);
        bool _Begin_Vl531l0x(Adafruit_VL53L0X* vl53l0x, uint8_t i2c_address, TwoWire* i2c_bus);
        bool _Begin_Apds9960(Adafruit_APDS9960* apds9960, uint8_t i2c_address, TwoWire* i2c_bus);
        void _Init_SPIFFS();
        void _Init_LittleFs();

    private:
        static uint8_t __i2c_bus_index;
        static uint8_t __ledc_channel_index;
};


class DevBoard: public BoardBase{
    public:
        void Init(const char* app_welcome_statement) override {
            this->_InitSerial(app_welcome_statement);
            Logger::Info("I am Dev-Board." );
            this->_Init_SPIFFS();
        };

};