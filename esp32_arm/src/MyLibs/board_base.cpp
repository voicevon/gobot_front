#include "board_base.h"

TwoWire* BoardBase::_Make_I2cBus(uint8_t pin_sda, uint8_t pin_scl, uint32_t frequency){
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


Adafruit_MCP23X17* BoardBase::_Make_Mcp23018(uint8_t i2c_address,TwoWire* i2c_bus){
    Adafruit_MCP23X17* mcp23018 = new Adafruit_MCP23X17();
    if(! mcp23018->begin_I2C(i2c_address, i2c_bus)){
        Serial.print("[Error] BoardBase::_MakeMcp23018() is failed! ");
        while (1);
    }
    Serial.println("[Info] BoardBase::_MakeMcp23018() is OK.");
    return mcp23018;
}

Adafruit_VL53L0X* BoardBase::_Make_Vl531l0x(uint8_t i2c_address, TwoWire* i2c_bus){
    Serial.println("[Info] BoardBase::_Make_Vl531l0x()");
    Adafruit_VL53L0X* vl53l0x = new Adafruit_VL53L0X();
    if(! vl53l0x->begin(i2c_address, true, i2c_bus)){
        Serial.println(F("Failed to boot VL53L0X"));
        while(1);
    }
    Serial.println(F("ObstacleSensor-VL53L0X is started.\n\n")); 
    return vl53l0x;
}

Adafruit_APDS9960* BoardBase::_Make_Apds9960(uint8_t i2c_address, TwoWire* i2c_bus){
    Serial.println("[Info] BoardBase::_Make_Apds9960()  ");
    Adafruit_APDS9960* apds9960 = new Adafruit_APDS9960();
    apds9960->begin(10, APDS9960_AGAIN_4X, APDS9960_ADDRESS, i2c_bus);
    apds9960->enableColor(true);
    return apds9960;

}

Adafruit_NeoPixel* BoardBase::_Make_Ws2812b(uint8_t pin_ws2812b){
    Serial.println("[Error] BoardBase::_Make_Ws2812b()  ");
    Adafruit_NeoPixel* ws2812b=new Adafruit_NeoPixel();
    return ws2812b;
}


