#include "mcp23018.h"
#define PIN_I2C_SCLK 23
#define PIN_I2C_DATA 22


Mcp23018::Mcp23018(){
    Wire.begin(PIN_I2C_DATA, PIN_I2C_SCLK, 400000);
    /*  The logic coil_id count is 52.
        The idea is 
            1. let logic_coil_id as a key.
            2. We can get chip_I2C_addr, pin_index inside chip, and next logic id
        Unfortunitely, we can NOT define an const array with greater than 128 bytes
        So, we define 3 individual ditionaries.
    */
    
    // uint8_t COIL_TABLE[52*3] = {
    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //0..4
    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //5..9
        
    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //10..14
    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //15..19
        
    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //20..24
    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //25..29

    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //30..34
    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //35..39

    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //40..44
    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //45..49

    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //50..54
    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //55..59

    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //60..64
    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //65..69

    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //70..74
    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //75..79

    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //80..84
    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //85..89

    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //90..94
    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //95..99

    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //100..104
    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //105..109
          
    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //110..104
    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //115..109
          
    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //120..104
    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //125..109
          
    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //130..104
    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //135..109
          
    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //140..104
    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //145..109
          
    //     0x34,2,2,  0x34,3,4, 0x35,4,6, 0x35,5,8,  0x35,6,10,  0x35,7,12,      //150..104
    //     0x34,2,2,                                                             //155
    //     }

    //Index is logic coil id, value is next coil logic id 
    uint8_t next_coil_id[3] = {3,22,76};
    memcpy(next_coil_id,__NextCoilId,sizeof(next_coil_id));
    
    //Index is logic coil id, value is I2c address, 
    uint8_t table_addr[5] = {0x45,0X45,0X45,0x46,0x5};
    memcpy(table_addr, __I2cAddress, sizeof(table_addr));

    //Index is logic coil id, value is phsical coil id inside mcp23018
    uint8_t table_pin_index[3] = {0,2,3};
    memcpy(table_pin_index,__PhysicalId,sizeof(table_pin_index));

    __CHIPS_COUNT = 5;


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