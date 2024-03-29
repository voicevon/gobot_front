#include "board_base.h"
#include "von/utility/logger.h"
#include "LittleFS.h"
// #include "SPIFFS.h"
#include "LittleFS.h"
#include "MyLibs/basic/memory_helper.h"

uint8_t BoardBase::__ledc_channel_index = 0;
uint8_t BoardBase::__i2c_bus_index = 0;

void BoardBase::_InitSerial(const char* welcome_statement){
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
    Serial.println(COLOR_RESET);
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




// void BoardBase::_Init_SPIFFS() {
// 	if (!SPIFFS.begin(true)) {
// 		Logger::Error("An error has occurred while mounting SPIFFS");
//         Logger::Halt("");
// 	}
// 	Logger::Info(" BoardBase::Init_SPIFFS() , SPIFFS mounted successfully");
//     Serial.print("SPIFFS Free: "); Serial.println(MemoryHelper::humanReadableSize((SPIFFS.totalBytes() - SPIFFS.usedBytes())));
//     Serial.print("SPIFFS Used: "); Serial.println(MemoryHelper::humanReadableSize(SPIFFS.usedBytes()));
//     Serial.print("SPIFFS Total: "); Serial.println(MemoryHelper::humanReadableSize(SPIFFS.totalBytes()));
// }

void BoardBase::_Init_LittleFs(){
    if (!LittleFS.begin(false /* false: Do not format if mount failed */)) {
        Serial.println("Failed to mount LittleFS");

        if (!LittleFS.begin(true /* true: format */)) {
            Logger::Error("An error has occurred while mounting LittleFs");
            Logger::Halt("");
        } else {
            Serial.println("LittleFS formatted successfully");
        }
    } else { // Initial mount success
    	Logger::Info(" BoardBase::_Init_LittleFs() , LittleFS mounted successfully");
    }
}

