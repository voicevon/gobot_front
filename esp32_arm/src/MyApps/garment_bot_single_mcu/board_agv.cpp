#include "board_agv.h"



void BoardPart_Agv::Init(TwoWire* i2c_bus_main, TwoWire* i2c_bus_ext){
    // Convert Dual apds9960 on dual i2c bus, to one track sensor
    this->_Begin_Apds9960(&this->__left_aps9960, I2C_ADDR_APDS9960, i2c_bus_main);
    this->_Begin_Apds9960(&this->__right_aps9960, I2C_ADDR_APDS9960, i2c_bus_ext);
    this->__dual_9960.Init(&this->__left_aps9960, &this->__right_aps9960);

    // Convert vl53l0x chip to obstacle sensor
    this->_Begin_Vl531l0x(&this->__vl53l0x, I2C_ADDR_VL53L0X, i2c_bus_ext);
    this->__obstacle_vl53l0x.Init(&this->__vl53l0x);
}

