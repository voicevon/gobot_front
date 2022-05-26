#pragma once
#ifdef USING_BOARD_AGV_SINGLE_BOARD_VER_2_0


// #define PIN_HCSR04_ECHO 18
// #define PIN_HCSR04_TRIG 19
#define PIN_WS2812B 13


// Common I2C bus is Connected to MCp23018
#define PIN_MAIN_I2C_SDA 27
#define PIN_MAIN_I2C_SCL 26
#define PIN_EXT_I2C_SDA 16
#define PIN_EXT_I2C_SCL 17 

#define PIN_WHEEL_PWM_LEFT 4
#define PIN_WHEEL_PWM_RIGHT 33
#define MC23018_PIN_WHEEL_DIR_LEFT 2
#define MC23018_PIN_WHEEL_DIR_RIGHT 25

#define PIN_ALPHA_STEP 22
#define MC23018_PIN_ALPHA_DIR 3
#define MC23018_PIN_ALPHA_ENABLE 2
#define PIN_BETA_STEP 32
#define MC23018_PIN_BETA_DIR 4
#define MC23018_PIN_BETA_ENABLE 5
#define MC23018_PIN_HOME_Z 7
#define MC23018_PIN_HOME_Y 6


#define PIN_RFID_SPI_CLK 18
#define PIN_RFID_SPI_MISO 19
#define PIN_RFID_SPI_MOSI 23
#define PIN_RFID_SS 5

#define PIN_BATTERY_VOLTAGE_ADC  34
#define PIN_CHARGER_VOLTAGE_ADC  35
#define PIN_IR_FRONT 36
#define PIN_IR_REAR 39

// #define MCP23018_PIN_EMERGENCY_STOP_BUTTON  11
// #define MCP23018_PIN_INPUT_BUTTON  11

#define WS2812B_COUNT 16

#define I2C_ADDR_MCP23018 0x23
#define I2C_ADDR_VL53L0X 0x29
#define I2C_ADDR_APDS9960 0x39  // Not changable.

#include <MyLibs/board_base.h>

class BoardSingleMcu_ver2_0: public BoardbaseCnc{
    public:
        BoardSingleMcu_ver2_0(){};
        void Init();
        void EnableMotor_alpha(bool enable_it) override;
        void EnableMotor_beta(bool enable_it) override;

        // TwoWire* GetI2c_mian(){return this->i2c_bus_main;};
        // TwoWire* GetI2c_ext(){return this->i2c_bus_ext;};
        Adafruit_MCP23X17* GetMcp23018(){return this->__mcp_23018;};
    private:
        TwoWire* __i2c_bus_main;
        TwoWire* __i2c_bus_ext;
        Adafruit_MCP23X17* __mcp_23018;
};

#endif