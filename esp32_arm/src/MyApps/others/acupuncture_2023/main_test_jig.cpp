//https://www.electronicshub.org/wp-content/uploads/2021/02/ESP32-Pinout-1.jpg
#include "i2c_master.h"
#include <string>
#include <HardwareSerial.h>

#include "all_applications.h"
#ifdef I_AM_ACUPUNCTURE_JIG_TESTER_2023

#define LED_COUNT 14

I2c_commu obj_i2c_bus = I2c_commu();

uint8_t LED_PINS[LED_COUNT] = {14,12,13,15, 2,4,5,18, 19,32,33,25, 26,27};
// uint8_t LED_PINS[LED_COUNT] = {14,12,13,15, 2,4,5,18, 19,32,33,25, 27,26};

void setup() {
    Serial.begin(115200);
    // setup i2c bus,   how many(and the list of slave i2c address) slaves are connected.
    obj_i2c_bus.Init(0, CELLS_COUNT);

    //setup led gpio
    for (int i=0; i< LED_COUNT; i++){
        pinMode(LED_PINS[i], OUTPUT);
    }
}


void loop() {
    TouchCell* pCell = obj_i2c_bus.FindandReadValidateCell();
    if (pCell == nullptr){
        Serial.println("Can not find any i2c slave address");
        return;
    }
    Serial.print("i2c address = ");
    Serial.print(pCell->Address);
    // Serial.print(cell_index );
    Serial.print("   ");
    
    Serial.print(pCell->CurrentFlags[0], BIN);
    Serial.print("   ");
    Serial.print(pCell->CurrentFlags[1], BIN);
    Serial.print("   ");
    Serial.print(pCell->CurrentFlags[2], BIN);
    Serial.print("   ");
    Serial.print(pCell->CurrentFlags[3], BIN);
    Serial.println(" ");
    for(int i=0; i < LED_COUNT; i++){
        uint8_t byte_index = 0;
        uint8_t bit_index = i;
        if (i>=8) {
            byte_index = 1;
            bit_index = i - 8;
        }
        bool flag = (pCell->CurrentFlags[byte_index] >> bit_index) & 0x01;
        digitalWrite(LED_PINS[i], flag);

    }
}
#endif