#include "board_base.h"
#include "Mylibs/basic/logger.h"
uint8_t BoardBase::__ledc_channel_index = 0;
uint8_t BoardBase::__i2c_bus_index = 0;

void BoardBase::_InitSerialBoard(const char* welcome_statement){
    Serial.begin(115200);
    Logger::Info(welcome_statement);
}


//  call   esp_task_wdt_reset() in the looo;
void BoardBase::EnableWatchdog(uint32_t WDT_TIMEOUT){
    esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
    esp_task_wdt_add(NULL); //add current thread to WDT watch
}


void BoardBase::RepportRamUsage(){
    Logger::Info("-----------------  RAM usage -----------------");
    Serial.print("Total PSRAM: ");
    Serial.println(ESP.getPsramSize());
    Serial.print("Free PSRAM: ");
    Serial.println(ESP.getFreePsram());

    Serial.print("total heap size = ");
    Serial.println(ESP.getHeapSize()); //total heap size

    Serial.print("available heap = ");
    Serial.println(ESP.getFreeHeap()); //available heap

    Serial.print("lowest level of free heap since boot = ");
    Serial.println (ESP.getMinFreeHeap()); //lowest level of free heap since boot

    Serial.print("largest block of heap that can be allocated at once = ");
    Serial.print(ESP.getMaxAllocHeap()); //largest block of heap that can be allocated at once
    Serial.println(FCBC_RESET);
}

uint8_t BoardBase::Assign_ledc_channel(){
    uint8_t channel= BoardBase::__ledc_channel_index;
    BoardBase::__ledc_channel_index ++;
    return channel;
}


bool BoardBase::_Begin_Mcp23018(Adafruit_MCP23X17* mcp23018, uint8_t i2c_address,TwoWire* i2c_bus){
    // Adafruit_MCP23X17* mcp23018 = new Adafruit_MCP23X17();
    if(! mcp23018->begin_I2C(i2c_address, i2c_bus)){
        Serial.print("[Error] BoardBase::_Begin_Mcp23018() is failed! ");
        while (1);
    }
    Serial.println("[Info] BoardBase::_Begin_Mcp23018() is OK.");
    return true;
}

bool BoardBase::_Begin_Vl531l0x(Adafruit_VL53L0X* vl53l0x, uint8_t i2c_address, TwoWire* i2c_bus){
    Serial.println("[Debug] BoardBase::_Begin_Vl531l0x()");
    // Adafruit_VL53L0X* vl53l0x = new Adafruit_VL53L0X();
    // if(! vl53l0x->begin(i2c_address, true, i2c_bus)){
    if(! vl53l0x->begin(i2c_address, false, i2c_bus)){
        Serial.println(F("[Error] Failed to boot VL53L0X"));
        while(1);
    }
    Serial.println(F("[Info] BoardBase::_Begin_Vl531l0x() ObstacleSensor-VL53L0X is started.\n\n")); 
    return true;
}

bool BoardBase::_Begin_Apds9960(Adafruit_APDS9960* apds9960, uint8_t i2c_address, TwoWire* i2c_bus){
    Serial.println("[Info] BoardBase::_Make_Apds9960()  ");
    // Adafruit_APDS9960* apds9960 = new Adafruit_APDS9960();
    apds9960->begin(10, APDS9960_AGAIN_4X, APDS9960_ADDRESS, i2c_bus);
    apds9960->enableColor(true);
    return apds9960;

}

// void BoardBase::SayHello(){
//     Serial.println("[Info] BoardBase::SayHello() Hello world.");
// }
// Adafruit_NeoPixel* BoardBase::_Make_Ws2812b(uint8_t pin_ws2812b){
//     Serial.println("[Error] BoardBase::_Make_Ws2812b()  ");
//     Adafruit_NeoPixel* ws2812b=new Adafruit_NeoPixel();
//     return ws2812b;
// }

