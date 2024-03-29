#include "board_all_in_one_2205.h"

void BoardAllInOne::Init(const char* app_welcome_statement){
    Serial.begin(115200);
    while (! Serial) {delay(1);}
    Serial.println("[Info] BoardAllInOne::Init()  Hi there, I am your lovely bot,  BotAsrsAgvCoreYZ, include ASRS AGV CNC.  Keep smiling :) ");
    this->RepportRamUsage();

    // this->_Begin_I2cBus(&this->__i2c_bus_main, PIN_MAIN_I2C_SDA_2205, PIN_MAIN_I2C_SCL_2205, 400000);
    // this->_Begin_I2cBus(&this->__i2c_bus_ext, PIN_EXT_I2C_SDA_2205, PIN_EXT_I2C_SCL_2205, 400000);


    this->_Begin_Mcp23018(&this->__mcp23018, I2C_ADDR_MCP23018_2205, &this->__i2c_bus_main);
    this->__mcp23018.pinMode(PIN_MCP23018_TEST_2205, OUTPUT);

    this->asrs.Init();
    // this->asrs.SayHello();
    this->asrs.LinkTwoWireBus(&this->__i2c_bus_ext);
    Serial.println("[Info] BoardAllInOne::Init() ASRS components is OK.");
    
    // Init cnc components 
    // this->cnc_board.Init();
    this->cnc_board.Init(&this->__mcp23018);
    this->cnc_board.EnableMotor(AXIS_ALPHA, false);
    this->cnc_board.EnableMotor(AXIS_BETA,false);
    Serial.println("[Info] BoardAllInOne::Init() CNC components is OK.");

    //Init agv components
    this->agv.Init();
    this->agv.Init(&this->__i2c_bus_main, &this->__i2c_bus_ext);
    Serial.println("[Info] BoardAllInOne::Init() AGV components is OK.");

    pinMode(PIN_BATTERY_VOLTAGE_ADC_2205, INPUT);

    // while(1)
    //     this->BlinkTest();
    Serial.println("[Info] BoardAllInOne::Init() ALL components is OK.");

}

void BoardAllInOne::Test_ScanI2cBuses(int loop_count){
    for (int i=0; i<loop_count; i++){
        // this->ScanI2cBus(&this->__i2c_bus_main, "bus_main");
        // this->ScanI2cBus(&this->__i2c_bus_ext, "bus_extended");
        delay(3000);           // wait 3 seconds for observing.
    }
}

void BoardAllInOne::Test_Blink(){
    Serial.print("Blinking...    >> ");
    Serial.println(blink_flag);
    this->__mcp23018.digitalWrite(PIN_MCP23018_TEST_2205, this->blink_flag);
    this->blink_flag = ! this->blink_flag;
    delay(2000);
}



// void BoardAllInOne::TurnLedOn(bool turn_on){
// 	// Light_WS2812B* led=new Light_WS2812B(WS2812B_COUNT, PIN_WS2812B);
// }

