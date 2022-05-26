#include "all_devices.h"
#ifdef USING_BOARD_AGV_SINGLE_BOARD_VER_2_0
#include "board_ver2.0.h"


void BoardSingleMcu_ver2_0::Init(){
    this->__i2c_bus_main = this->_Make_I2cBus(PIN_MAIN_I2C_SDA, PIN_MAIN_I2C_SCL, 400000);
    this->__i2c_bus_ext = this->_Make_I2cBus(PIN_EXT_I2C_SDA, PIN_EXT_I2C_SCL, 400000);

    bool scan_buses = false;
    if (scan_buses){
        this->ScanI2cBus(__i2c_bus_main, "bus_main");
        this->ScanI2cBus(__i2c_bus_ext, "bus_extended");
        delay(3000);           // wait 5 seconds for next scan
    }
    this->__mcp23018 = this->_Make_Mcp23018(I2C_ADDR_MCP23018, this->__i2c_bus_main);
    this->__mcp23018->pinMode(MC23018_PIN_ALPHA_ENABLE, OUTPUT);
    this->__mcp23018->pinMode(MC23018_PIN_BETA_ENABLE, OUTPUT);
    this->__mcp23018->pinMode(PIN_MCP23018_TEST, OUTPUT);

    this->__vl53l0x = this->_Make_Vl531l0x(I2C_ADDR_VL53L0X, this->__i2c_bus_ext);
    this->__left_aps9960 = this->_Make_Apds9960(I2C_ADDR_APDS9960, this->__i2c_bus_main);
    this->__right_aps9960 = this->_Make_Apds9960(I2C_ADDR_APDS9960, this->__i2c_bus_ext);


    this->EnableMotor_alpha(false);
    this->EnableMotor_beta(false);

    pinMode(PIN_BATTERY_VOLTAGE_ADC, INPUT);

    // while(1)
    //     this->BlinkTest();

}

void BoardSingleMcu_ver2_0::BlinkTest(){
    Serial.print("Blinking...    >> ");
    Serial.println(blink_flag);
    this->__mcp23018->digitalWrite(PIN_MCP23018_TEST, this->blink_flag);
    this->blink_flag = ! this->blink_flag;
    delay(2000);
}

void BoardSingleMcu_ver2_0::EnableMotor_alpha(bool enable_it){
    this->__mcp23018->digitalWrite(MC23018_PIN_ALPHA_ENABLE, !enable_it);   // LOW is enable
}

void BoardSingleMcu_ver2_0::EnableMotor_beta(bool enable_it){
    this->__mcp23018->digitalWrite(MC23018_PIN_BETA_ENABLE, !enable_it);   // LOW is enable
}

float BoardSingleMcu_ver2_0::Get_Battery_volt(){
    int16_t adc = analogRead(PIN_BATTERY_VOLTAGE_ADC);
    // translate adc to voltage
    float volt = 0.123 * adc + 12.22 ;
    return volt;
}

void BoardSingleMcu_ver2_0::TurnLedOn(bool turn_on){
	// Light_WS2812B* led=new Light_WS2812B(WS2812B_COUNT, PIN_WS2812B);
}



#endif