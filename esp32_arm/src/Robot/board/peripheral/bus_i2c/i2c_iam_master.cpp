
#include "i2c_iam_master.h"
#include <Arduino.h>
#include "MyLibs/basic/logger.h"


void I2C_IamMaster::Init(){
    // Using standard pin of SDA= GPIO-26?  SCL=GPIO-27 ?
    bool i2c_is_ok = false; 
    while (!i2c_is_ok){
        i2c_is_ok = Wire.begin();
        if (! i2c_is_ok){
            Logger::Error("I2c begin failed!");
        }
    }
} 


// return:  true A: the cell is offline before entring.
//               B: the cell is still online
//          false:  the cell is from online to offline.
bool I2C_IamMaster::ReadSlaveNode(I2C_SlaveNodeAgent* slave_node){
    if (!slave_node->IsOnline()) return true;   //????  TODO: solve this.
    // Logger::Debug("I2C_IamMaster::ReadSlaveNode()");
    // Logger::Print("slave_node address", slave_node->Address);
    Wire.beginTransmission(slave_node->Address);
    Wire.endTransmission(false);
    Wire.requestFrom(slave_node->Address, slave_node->RxBufferSize);    // request data from slave device
    int index = 0;
    uint8_t* rx_buffer = slave_node->GetRxBuffer(); 
    while (Wire.available() > 0) {  // slave may send less bytes than expected.
        *(rx_buffer+index) = Wire.read();
        // Serial.print(*(rx_buffer + index));
        // Serial.print("\t");
        index++;
    }
    // Serial.println();
    if(index == 0) {
        // Can not read any data from i2c bus  for this address.
        Logger::Print("Turn to offline node_address", slave_node->Address);
        slave_node->SetOffline();

    } 
    Wire.endTransmission(true);
    return true;
}

