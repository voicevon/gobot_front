
#include "i2c_master.h"
// #include "HardwareSerial.h"
#include <Arduino.h>
#include "MyLibs/basic/logger.h"
// #include "Mqtt/from_mqtt_client_to_remote_queue.h"

#define BYTES_COUNT_FOR_COMMU 14   // TODO:  configable.

// void I2C_Master::Init(int min_cell_i2c_address, int cells_count){
void I2C_Master::Init(){
    pinMode(2, OUTPUT);
    bool i2c_is_ok = false; 
    while (!i2c_is_ok){
        digitalWrite(2, LOW);
        i2c_is_ok = Wire.begin();
        if (! i2c_is_ok){
            Logger::Error("I2c begin failed!");
            digitalWrite(2, HIGH);
            delay(500);
        }
    }

} 




// return:  true A: the cell is offline before entring.
//               B: the cell is still online
//          false:  the cell is from online to offline.
bool I2C_Master::ReadSlaveNode(I2C_SlaveNode* slave_node){

    if (!slave_node->IsOnline) return true;

    // uint8_t bytes_toread = 4;
    Wire.beginTransmission(slave_node->I2C_Address);
    Wire.endTransmission(false);
    Wire.requestFrom(slave_node->I2C_Address, BYTES_COUNT_FOR_COMMU);    // request data from slave device
    int index = 0;
    uint8_t* rx_buffer = slave_node->GetRxBuffer(); 
    while (Wire.available() > 0) {  // slave may send less bytes than expected.
        *(rx_buffer+index) = Wire.read();
        index++;
    }
    if(index == 0) {
        // Can not read any data from i2c bus  for this address.
        slave_node->SetOffline();
        if (slave_node->IsForceOnline){
            // g_mqttClient.publish("actp/001/offline", 2, false, String(cell->Address).c_str());
            digitalWrite(2, HIGH);
            delay(500);
            digitalWrite(2,LOW);
            Serial.println("\n I2C_Master::ReadSingleCell()  No response.  cell_address= " + String(slave_node->I2C_Address));
            Wire.endTransmission(true);
            return false;
        }

    } 
    Wire.endTransmission(true);
    return true;
    // delay(1000);
}

