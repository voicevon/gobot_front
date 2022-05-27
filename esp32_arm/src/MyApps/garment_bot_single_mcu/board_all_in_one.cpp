#include  "board_all_in_one.h"

void BoardAllInOne::Init(){
    this->_Begin_I2cBus(&this->__i2c_bus_main, PIN_MAIN_I2C_SDA, PIN_MAIN_I2C_SCL, 400000);
    this->_Begin_I2cBus(&this->__i2c_bus_ext, PIN_EXT_I2C_SDA, PIN_EXT_I2C_SCL, 400000);

    bool scan_buses = false;
    if (scan_buses){
        this->ScanI2cBus(&this->__i2c_bus_main, "bus_main");
        this->ScanI2cBus(&this->__i2c_bus_ext, "bus_extended");
        delay(3000);           // wait 5 seconds for next scan
    }
    this->_Begin_Mcp23018(&this->__mcp23018, I2C_ADDR_MCP23018, &this->__i2c_bus_main);
    this->__mcp23018.pinMode(MC23018_PIN_ALPHA_ENABLE, OUTPUT);
    this->__mcp23018.pinMode(MC23018_PIN_BETA_ENABLE, OUTPUT);
    this->__mcp23018.pinMode(PIN_MCP23018_TEST, OUTPUT);
    // Init cnc components 
    this->cnc.Init(&this->__mcp23018);
    this->cnc.EnableMotor_alpha(false);
    this->cnc.EnableMotor_beta(false);
    Serial.println("[Info] BoardAllInOne::Init() CNC components is OK.");

    //Init agv components
    this->agv.Init(&this->__i2c_bus_main, &this->__i2c_bus_ext);
    Serial.println("[Info] BoardAllInOne::Init() AGV components is OK.");

    pinMode(PIN_BATTERY_VOLTAGE_ADC, INPUT);

    // while(1)
    //     this->BlinkTest();
    Serial.println("[Info] BoardAllInOne::Init() ALL components is OK.");

}

void BoardAllInOne::BlinkTest(){
    Serial.print("Blinking...    >> ");
    Serial.println(blink_flag);
    this->__mcp23018.digitalWrite(PIN_MCP23018_TEST, this->blink_flag);
    this->blink_flag = ! this->blink_flag;
    delay(2000);
}



float BoardAllInOne::Get_Battery_volt(){
    int16_t adc = analogRead(PIN_BATTERY_VOLTAGE_ADC);
    // translate adc to voltage
    float volt = 0.123 * adc + 12.22 ;
    return volt;
}

void BoardAllInOne::TurnLedOn(bool turn_on){
	// Light_WS2812B* led=new Light_WS2812B(WS2812B_COUNT, PIN_WS2812B);
}


