
#include "i2c_master.h"
#include "HardwareSerial.h"
#include <Arduino.h>
#include "MyLibs/basic/logger.h"
#include "Mqtt/from_mqtt_client_to_remote_queue.h"

#define BYTES_COUNT_FOR_COMMU 16

void I2C_Master::Init(int min_cell_i2c_address, int cells_count){
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
    for(int i=0; i< cells_count; i++){
        Cells[i].I2C_Address = min_cell_i2c_address + i;
    }
    this->__CELLS_COUNT = cells_count;
    // this->__CELL_I2C_ADDRESS_MIN = min_cell_i2c_address;
} 

// bool I2C_Master::HasUpdate(){
//     return this->_has_update;
// }


// return:  true A: the cell is offline before entring.
//               B: the cell is still online
//          false:  the cell is from online to offline.
bool  I2C_Master::ReadSingleCell(TouchCell_2023* cell){
    if (!cell->IsOnline) return true;

    // uint8_t bytes_toread = 4;
    Wire.beginTransmission(cell->I2C_Address);
    Wire.endTransmission(false);
    Wire.requestFrom(cell->I2C_Address, BYTES_COUNT_FOR_COMMU);    // request data from slave device
    int index = 0;
    uint8_t* rx_buffer = cell->GetRxBuffer(); 
    while (Wire.available() > 0) {  // slave may send less bytes than expected.
        // uint8_t received_byte = Wire.read();         // receive a byte as character
        // cell->CurrentFlags[index] = received_byte;
        *(rx_buffer+index) = Wire.read();
        index++;
    }
    if(index == 0) {
        // Can not read any data from i2c bus  for this address.
        cell->SetOffline();
        if (cell->IsForceOnline){
            // g_mqttClient.publish("actp/001/offline", 2, false, String(cell->Address).c_str());
            digitalWrite(2, HIGH);
            delay(500);
            digitalWrite(2,LOW);
            Serial.println("\n I2C_Master::ReadSingleCell()  No response.  cell_address= " + String(cell->I2C_Address));
            Wire.endTransmission(true);
            return false;
        }

    } 
    Wire.endTransmission(true);
    return true;
    // delay(1000);
}

// void I2C_Master::SpinOnce(){
//     for (uint8_t i= 0; i< this->__CELLS_COUNT; i++){
//         TouchCell_2023* pCell = &(this->Cells[i]);
//         if (pCell->IsOnline){
//             this->ReadSingleCell(pCell);
//         }
//     }
//     // bool all_is_offline = false;
//     for(int i=0; i< this->__CELLS_COUNT; i++){
//         if (this->Cells[i].IsOnline)
//             return;
//     }


//     //All cells is offline. re init all cells to online
//     Logger::Info("Reset all cells state to online");
//     for(int i=0; i< this->__CELLS_COUNT; i++){
//         this->Cells[i].SetOnline();
//         Logger::Print("address", Cells[i].Address);
//     }
//     digitalWrite(2, LOW);
//     // for (int i=0; i<__CELLS_COUNT;i++){
//     // Serial.print("Min_address= ");
//     // Serial.print(this->Cells[0].Address);
//     // Serial.print("   CELLS_COUNT= ");
//     // Serial.print(this->__CELLS_COUNT);
//     // Serial.println(FCBC_RESET);
//     // }
    
// }

TouchCell_2023* I2C_Master::FindandReadValidateCell(){
    for (uint8_t i= 0; i< this->__CELLS_COUNT; i++){
        TouchCell_2023* pCell = &(this->Cells[i]);
        if (pCell->IsOnline){
            // pCell->PrintOut("Trying to read a cell");
            // pCell->CopyCurrentToLast();
            this->ReadSingleCell(pCell);
            // pCell->CompareCurrentAndLast();
            if (pCell->IsOnline){
                return pCell;
            }
        }
    }
    //All cells is offline. re init all cells to online
    for(int i=0; i< this->__CELLS_COUNT; i++){
        this->Cells[i].SetOnline();
    }
    // }
    return nullptr;
}
