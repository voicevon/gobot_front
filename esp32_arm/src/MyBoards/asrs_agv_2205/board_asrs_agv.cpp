#include  "board_asrs_agv.h"


void BoardAllInOne::Init(bool is_on_reset){
    Serial.begin(115200);
    while (! Serial) {delay(1);}
    Serial.println("[Info] BoardAllInOne::Init()  Hi there, I am your lovely bot,  BotAsrsAgvCoreYZ, include ASRS AGV CNC.  Keep smiling :) ");
    this->RepportRamUsage();

    this->_Begin_I2cBus(&this->__i2c_bus_main, PIN_MAIN_I2C_SDA_2205, PIN_MAIN_I2C_SCL_2205, 400000);
    this->_Begin_I2cBus(&this->__i2c_bus_ext, PIN_EXT_I2C_SDA_2205, PIN_EXT_I2C_SCL_2205, 400000);

    bool scan_buses = false;
    if (scan_buses){
        this->ScanI2cBus(&this->__i2c_bus_main, "bus_main");
        this->ScanI2cBus(&this->__i2c_bus_ext, "bus_extended");
        delay(3000);           // wait 5 seconds for next scan
    }
    this->_Begin_Mcp23018(&this->__mcp23018, I2C_ADDR_MCP23018_2205, &this->__i2c_bus_main);
    this->__mcp23018.pinMode(MC23018_PIN_ALPHA_ENABLE_2205, OUTPUT);
    this->__mcp23018.pinMode(MC23018_PIN_BETA_ENABLE_2205, OUTPUT);
    this->__mcp23018.pinMode(PIN_MCP23018_TEST_2205, OUTPUT);
    // Init cnc components 
    this->cnc.Init(false);
    this->cnc.Init(&this->__mcp23018);
    // this->cnc.EnableMotor_alpha(false);
    // this->cnc.EnableMotor_beta(false);
    this->cnc.EnableMotor('A', false);
    this->cnc.EnableMotor('B',false);
    Serial.println("[Info] BoardAllInOne::Init() CNC components is OK.");

    //Init agv components
    this->agv.Init(false);
    this->agv.Init(&this->__i2c_bus_main, &this->__i2c_bus_ext);
    Serial.println("[Info] BoardAllInOne::Init() AGV components is OK.");

    pinMode(PIN_BATTERY_VOLTAGE_ADC_2205, INPUT);

    // while(1)
    //     this->BlinkTest();
    Serial.println("[Info] BoardAllInOne::Init() ALL components is OK.");

}

void BoardAllInOne::BlinkTest(){
    Serial.print("Blinking...    >> ");
    Serial.println(blink_flag);
    this->__mcp23018.digitalWrite(PIN_MCP23018_TEST_2205, this->blink_flag);
    this->blink_flag = ! this->blink_flag;
    delay(2000);
}



float BoardAllInOne::Get_Battery_volt(){
    int16_t adc = analogRead(PIN_BATTERY_VOLTAGE_ADC_2205);
    // translate adc to voltage
    float volt = 0.123 * adc + 12.22 ;
    return volt;
}

void BoardAllInOne::TurnLedOn(bool turn_on){
	// Light_WS2812B* led=new Light_WS2812B(WS2812B_COUNT, PIN_WS2812B);
}

