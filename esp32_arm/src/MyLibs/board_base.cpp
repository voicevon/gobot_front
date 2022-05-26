#include "board_base.h"

TwoWire* BoardBase::_MakeI2cBus(uint8_t pin_sda, uint8_t pin_scl, uint32_t frequency){
    TwoWire* new_bus = new TwoWire(this->__i2c_bus_index);
    new_bus->begin(pin_sda, pin_scl, frequency);
    this->__i2c_bus_index++;
    return new_bus;
}

void BoardBase::ScanI2cBus(TwoWire* i2c_bus, const char* printing_topic){
    Serial.print("[Info] BoardBase::ScanI2cBus()   Scanning  >>> ");
    Serial.println(printing_topic);

    uint8_t error, address;
    int nDevices = 0;
    for(address = 1; address < 127; address++ )
    {
        // The i2c_scanner uses the return value of
        // the Write.endTransmisstion to see if
        // a device did acknowledge to the address.
        i2c_bus->beginTransmission(address);
        error = i2c_bus->endTransmission();
        if (error == 0)
        {
        Serial.print("I2C device found at address 0x");
        if (address<16)
            Serial.print("0");
        Serial.print(address,HEX);
        Serial.println("  !");
        nDevices++;
        }
        else if (error==4)
        {
        Serial.print("Unknown error at address 0x");
        if (address<16)
            Serial.print("0");
        Serial.println(address,HEX);
        }    
    }
    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("done\n");
}


Adafruit_MCP23X17* BoardBase::_MakeMcp23018(uint8_t i2c_address,TwoWire* i2c_bus){
    this->__mcp23018 = new Adafruit_MCP23X17();
    if(!this->__mcp23018->begin_I2C(i2c_address, i2c_bus)){
        Serial.print("[Error] BoardBase::_MakeMcp23018() is failed! ");
        while (1);
    }
    Serial.println("[Info] BoardBase::_MakeMcp23018() is OK.");
}


