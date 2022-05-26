#include "all_devices.h"
#ifdef USING_BOARD_AGV_SINGLE_BOARD_VER_2_0
#include "board_ver2.0.h"


void BoardSingleMcu_ver2_0::Init(){
    this->__i2c_bus_main = this->_MakeI2cBus(PIN_COMMON_I2C_SDA, PIN_COMMON_I2C_SCL, 400000);
    this->__i2c_bus_ext = this->_MakeI2cBus(PIN_EXT_I2C_SDA, PIN_EXT_I2C_SCL, 400000);
}

void BoardSingleMcu_ver2_0::EnableMotor_alpha(bool enable_it){
    this->__mcp_23018->digitalWrite(MC23018_PIN_ALPHA_ENABLE, !enable_it);
}

void BoardSingleMcu_ver2_0::EnableMotor_beta(bool enable_it){
    this->__mcp_23018->digitalWrite(MC23018_PIN_BETA_ENABLE, !enable_it);
}

#endif