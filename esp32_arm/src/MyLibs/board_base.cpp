#include "board_base.h"

TwoWire* BoardBase::_MakeI2cBus(uint8_t pin_sda, uint8_t pin_scl, uint32_t frequency){
    TwoWire* new_bus = new TwoWire(this->__i2c_bus_index);
    new_bus->begin(pin_sda, pin_scl, frequency);
    this->__i2c_bus_index++;
    return new_bus;
}

void BoardBase::ScanI2cBus(TwoWire* i2c_bus){

}


Adafruit_MCP23X17* BoardBase::_MakeMcp23018(TwoWire* i2c_bus){

}


