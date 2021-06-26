#include "mcp23018.h"
#define I2C_SCLK 5
#define I2C_DATA 6


Mcp23018::Mcp23018(){
    Wire.begin(I2C_DATA, I2C_SCLK, 400000);
    //Index is logic coil id, value is I2c address
    uint8_t table_addr[5] = {0x1,0x2,0x3,0x4,0x5};
    memcpy(table_addr, __I2cAddress, sizeof(table_addr));
    //Index is logic coil id, value is phsical coil id inside mcp23018
    uint8_t table_index[3] = {1,2,3};
    memcpy(table_index,__PhysicalId,sizeof(table_index));
    __CHIPS_COUNT = 9;
}

void Mcp23018::DisableAllCoils(){
    for (int i=0;i<__CHIPS_COUNT;i++){
        uint16_t addr = __I2cAddress[i];
        Wire.writeTransmission(addr, &__UINT8_ZERO,1,true);
    }
}
void Mcp23018::EnableSingleCoil(int logic_coil_id, bool enable_it){
    static int last_enabled_logic_coil_id;
    // Prepare the data value of the action.
    uint8_t last_addr = __I2cAddress[last_enabled_logic_coil_id];
    uint8_t this_addr = __I2cAddress[logic_coil_id];
    uint8_t bit_index_inside_chip = __PhysicalId[logic_coil_id];
    uint8_t value = 0;
    if (enable_it)
        value = 1 << bit_index_inside_chip;
    
    // Write out the data value to the coils.
    if (last_addr != this_addr){
        // Disable last coil
        Wire.writeTransmission(last_addr, &__UINT8_ZERO,1,true);
    }
    // Enable new coil. mean while, disable the last coil if they are at same addr.
    Wire.writeTransmission(this_addr,&value,1,true);

    // prepair next.
    last_enabled_logic_coil_id = logic_coil_id;

  }