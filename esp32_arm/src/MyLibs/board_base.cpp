#include "board_base.h"

PcbaBoardBase::MakeI2cBus* MakerI2cBus(uint8_t pin_sda, uint8_t pin_scl, uint16_t frequency){
    TwoWire* new_bus = new TwoWire(pin_sda, pin_scl, frequency);
    return new_bus;
}

void PcbaBoardBase::ScanI2cBus(TwoWire* i2c_bus){

}
