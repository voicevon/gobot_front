#include "board_all_in_one_2205.h"

void BoardAllInOne::Init(bool is_on_reset){
    Serial.begin(115200);
    while (! Serial) {delay(1);}
    Serial.println("[Info] BoardAllInOne::Init()  Hi there, I am your lovely bot,  BotAsrsAgvCoreYZ, include ASRS AGV CNC.  Keep smiling :) ");
    this->RepportRamUsage();

    this->_Begin_I2cBus(&this->__i2c_bus_main, PIN_MAIN_I2C_SDA_2205, PIN_MAIN_I2C_SCL_2205, 400000);
    this->_Begin_I2cBus(&this->__i2c_bus_ext, PIN_EXT_I2C_SDA_2205, PIN_EXT_I2C_SCL_2205, 400000);


    this->_Begin_Mcp23018(&this->__mcp23018, I2C_ADDR_MCP23018_2205, &this->__i2c_bus_main);
    this->__mcp23018.pinMode(PIN_MCP23018_TEST_2205, OUTPUT);

    this->asrs.Init(false);
    // this->asrs.SayHello();
    this->asrs.LinkTwoWireBus(&this->__i2c_bus_ext);
    Serial.println("[Info] BoardAllInOne::Init() ASRS components is OK.");
    
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

void BoardAllInOne::Test_ScanI2cBuses(int loop_count){
    for (int i=0; i<loop_count; i++){
        this->ScanI2cBus(&this->__i2c_bus_main, "bus_main");
        this->ScanI2cBus(&this->__i2c_bus_ext, "bus_extended");
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

void BoardAllInOne::Test_Sharp_IrSensor(int loop_count){
    Serial.println("[Info] BoardAllInOne::Test_Sharp_IrSensor() is entering.");
    JettySensor_SharpIrOnAds1115* sharp = this->asrs.GetJettySensor();
    for (int i =0; i<loop_count; i++){
        float a = sharp->ReadDistance(JettySensorBase::IR_POSITION::LOWER_INNER);
        float b = sharp->ReadDistance(JettySensorBase::IR_POSITION::LOWER_OUTER);
        float c = sharp->ReadDistance(JettySensorBase::IR_POSITION::UPPER_INNER);
        float f = sharp->ReadDistance(JettySensorBase::IR_POSITION::UPPER_OUTER);
        Serial.print(a);
        Serial.print("\t");
        Serial.print(b);
        Serial.print("\t");
        Serial.print(c);
        Serial.print("\t");
        Serial.println(f);
        delay(300);
    }
}

void BoardAllInOne::Test_TrackSensor(int loop_count){
    TrackSensorBase* sensor = this->agv.GetTrackSensor();
    for (int i=0; i<loop_count; i++){
        int16_t error = sensor->ReadAlignmentError();
        Serial.println(error);
        delay(200);
    }
}

void BoardAllInOne::Test_AllHomers(int loop_count){
    SingleAxisHomer* z = this->cnc.GetHomer('Z');
    SingleAxisHomer* y = this->cnc.GetHomer('Y');
    bool zh,yh;
    for(int i=0; i<loop_count; i++){
        zh= z->IsTriged();
        yh = y->IsTriged();
        Serial.print("z: ");
        Serial.print(zh);
        Serial.print("\ty: 444");
        Serial.println(yh);
        delay(200);
    }
    
}
void BoardAllInOne::Test_Stepper(int loop_count, char axis_name, int distance_in_step, StepControl* control){
    Stepper* ss = this->cnc.GetStepper(axis_name);
    this->cnc.EnableMotor(axis_name, true);
    ss->setAcceleration(distance_in_step / 2);
    for(int i=0; i<loop_count; i++){
        //CW
        ss->setTargetAbs(distance_in_step);
        control->move(*ss);
        delay(500);
        //CCW
        ss->setTargetAbs(0);
        control->move(*ss);
        delay(500);
    }
}

// void BoardAllInOne::Test_CncHome(char axis_name, int loop_count){
// }


void BoardAllInOne::Test_ObstacleSensor(int loop_count){
    ObstacleSensorBase* sensor = this->agv.GetObstacleSensor();
    for(int i=0; i<loop_count; i++){
        bool blocked = sensor->DetectObstacle();
        Serial.println(blocked);
        delay(200);
    }
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

