#include "i2c_tool.h"
#include "HardwareSerial.h"


bool I2C_Tool::_Begin_I2cBus(TwoWire* i2c_bus, uint8_t pin_sda, uint8_t pin_scl, uint32_t frequency){
    // TwoWire* new_bus = new TwoWire(this->__i2c_bus_index);
    if (!i2c_bus->begin(pin_sda, pin_scl, frequency)){
        Serial.println("[Error]  BoardBase::_Begin_I2cBus(), failed.");
        while(1);        
    }
    Serial.println("[Info] BoardBase::_Begin_I2cBus() is OK.");
    return true;

    // this->__i2c_bus_index++;
    // return new_bus;
}

void I2C_Tool::ScanI2cBus(TwoWire* i2c_bus, const char* printing_topic){
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
