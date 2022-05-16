#pragma once
#ifdef I_AM_GARMENT_BOT_SINGLE_MCU




#define PIN_HCSR04_ECHO 18
#define PIN_HCSR04_TRIG 19
#define PIN_WS2812B 11

#define PIN_COMMON_I2C_SDA 21
#define PIN_COMMON_I2C_SCL 22
#define PIN_RIGHT_APDS_9960_SDA 23
#define PIN_RIGHT_APDS_9960_SCL 15

#define PIN_WHEEL_PWM_LEFT 11
#define PIN_WHEEL_PWM_RIGHT 11
#define MC23018_PIN_WHEEL_DIR_LEFT 3
#define MC23018_PIN_WHEEL_DIR_RIGHT 4

#define PIN_ALPHA_STEP 11
#define MC23018_PIN_ALPHA_DIR 11
#define MC23018_PIN_ALPHA_ENABLE 11
#define PIN_BETA_STEP 11
#define MC23018_PIN_BETA_DIR 11
#define MC23018_PIN_BETA_ENABLE 11
#define MC23018_PIN_HOME_Z 11
#define MC23018_PIN_HOME_Y 11


#define PIN_RFID_SPI_CLK 11
#define PIN_RFID_SPI_MISO 11
#define PIN_RFID_SPI_MOSI 11

#define PIN_BATTERY_VOLTAGE_ADC  34
#define PIN_CHARGER_VOLTAGE_ADC  35
#define PIN_IR_FRONT 36
#define PIN_IR_REAR 39

#define MCP23018_PIN_EMERGENCY_STOP_BUTTON  11
#define MCP23018_PIN_INPUT_BUTTON  11

#define WS2812B_COUNT 16

#endif