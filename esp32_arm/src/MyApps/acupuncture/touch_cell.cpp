#include "touch_cell.h"
#include "HardwareSerial.h"

#define PPD2 0
#define PPD3 1
#define PPD4 2
#define PPD5 3
#define PPD6 4
#define PPD7 5
#define PPD8 6
#define PPD9 7
#define PPD10 8
#define PPD11 9

#define PPA0 10   // D14
#define PPA1 11   // D15
#define PPA2 12   // D16
#define PPA3 13   // D17




bool TouchCell::HasUpdate(){
    return this->_has_update;
}

const char* TouchCell::GetName(int point_id){
    String name = "u";
    name.concat(this->Address);
    name.concat("p");
    name.concat(point_id);
    name.toCharArray(this->__mqtt_topic_substring, 30);
    return this->__mqtt_topic_substring;
}


bool TouchCell::CompareCurrentAndLast(){
    for(int i=0; i<4; i++){
        if (LastFlags[i] != CurrentFlags[i]){
            Serial.println("Some touch pad is changed.");
            this->_has_update = true;
            return true;   
        }
    }
    this->_has_update = false;
    return false;
}

void TouchCell::CopyCurrentToLast(){
    for(int i =0; i<4; i++){
        this->LastFlags[i] = this->CurrentFlags[i];
    }
}
bool TouchCell::IsBitUpdated(int bit_index){
    int byte_index = 0;
    if (bit_index>=8){
        byte_index++;
        bit_index -=8;
    }
    if ((CurrentFlags[byte_index] ^ LastFlags[byte_index]) & (1<<bit_index))
        return true;
    return false;
}
const char* TouchCell::GetMqttPayload(int bit_index){
    int byte_index = 0;
    if (bit_index >= 8){
        byte_index++;
        bit_index -= 8;
    }
    if (CurrentFlags[byte_index] & (1<<bit_index))
        return "ON";
    return "OFF";
}

void TouchCell::PrintOut(const char* title){
    return;
    Serial.print("\n======================= TouchCell::PrintOut()===================\n");
    Serial.println(title);
    Serial.print("\n    Addr = ");
    Serial.print(this->Address);

}
