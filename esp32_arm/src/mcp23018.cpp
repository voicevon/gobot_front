#include "mcp23018.h"
#define I2C_SCLK 5
#define I2C_DATA 6


Mcp23018::Mcp23018(){
    Wire.begin(I2C_DATA, I2C_SCLK, 400000);
    // __I2cAddress = [0x23,0x24,0x25,0x26];   //Index is logic coil id, value is I2c address
    // __PhysicalId = {1,2,3,4,5,7,1,2,3,4,5,6,7,1,2,3,4,5,6,1,2,3,4,5};  //Index is logic coil id, value is phsical coil id inside mcp23018
}

void Mcp23018::EnableSingleCoil(int logic_coil_id, bool enable_it){
    static int last_enabled_logic_coil_id;
    //disable last_coil
    uint8_t addr = __I2cAddress[logic_coil_id];

    //enable current coil

    last_enabled_logic_coil_id = logic_coil_id;

  }