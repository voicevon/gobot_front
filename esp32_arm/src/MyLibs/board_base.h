#pragma once

#include <HardwareSerial.h>
// #ifdef BOARD_WIRE
    #include <Wire.h>
// #endif

#include <Adafruit_MCP23X17.h>
#include <Adafruit_VL53L0X.h>
#include <Adafruit_APDS9960.h>
#include <Adafruit_NeoPixel.h>


class BoardBase{
    public:
        void ScanI2cBus(TwoWire* i2c_bus, const char* printing_topic);
        static uint8_t Assign_ledc_channel(){return __ledc_channel_index; __ledc_channel_index++;};
        static uint8_t Assign_i2c_bus_id(){return __i2c_bus_index; __i2c_bus_index++;};

    protected:
        bool _Begin_I2cBus(TwoWire* i2c_bus, uint8_t pin_sda, uint8_t pin_scl, uint32_t frequency);
        bool _Begin_Mcp23018(Adafruit_MCP23X17* mcp, uint8_t i2c_address, TwoWire* i2c_bus);
        bool _Begin_Vl531l0x(Adafruit_VL53L0X* vl53l0x, uint8_t i2c_address, TwoWire* i2c_bus);
        bool _Begin_Apds9960(Adafruit_APDS9960* apds9960, uint8_t i2c_address, TwoWire* i2c_bus);
        // bool _Make_Ws2812b(uint8_t pin_ws2812b);

    private:
        static uint8_t __i2c_bus_index;
        static uint8_t __ledc_channel_index;
};


// #include "ESP32Step/src/Stepper.h"
#include "ESP32Step/src/TeensyStep.h"

class BoardbaseCnc: public BoardBase{
    public:
        virtual void EnableMotor_alpha(bool enable_it);
        virtual void EnableMotor_beta(bool enable_it);
        void SayHello();

    protected:
        // use of deleted function 'StepControlBase<a, t>::StepControlBase(const StepControlBase<a, t>&) [with Accelerator = LinStepAccelerator; TimerField = TimerField]'
        // StepControl* GetStepControl(){return &this->_objStepControl;};
        // StepControl _objStepControl;

    private:

};


class BoardbaseAgv: public BoardBase{
    public:

    protected:

    private:

    
};
