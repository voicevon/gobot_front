
#include "i2c_commu.h"
#include "HardwareSerial.h"
#include <Arduino.h>
#include "MyLibs/basic/logger.h"


void I2c_commu::Init(int min_cell_i2c_address, int cells_count){
    bool i2c_is_ok = Wire.begin(); 
    if(!i2c_is_ok){
        Logger::Error("I2c begin failed!");
        while (true){
            Serial.print("E ");
            delay(500);
        }
    }
    for(int i=0; i< cells_count; i++){
        Cells[i].Address = min_cell_i2c_address + i;
    }
    this->__CELLS_COUNT = cells_count;
    this->__CELL_I2C_ADDRESS_MIN = min_cell_i2c_address;
} 

bool I2c_commu::HasUpdate(){
    return this->_has_update;
}

void I2c_commu::ReadSingleCell(TouchCell* cell){
    if (!cell->IsOnline) return;
    // Serial.println("\n     I2c_commu::ReadSingleCell() is Entering. cell_address= " + cell->Address);
    // cell->PrintOut("I2c_commu::ReadSingleCell() is Entering.");

    uint8_t n_bytes = 4;
    // cell->Address = 3;
    Wire.beginTransmission(cell->Address);
    Wire.endTransmission(false);
    Wire.requestFrom(cell->Address, n_bytes);    // request data from slave device
    int i=0;
    while (Wire.available() > 0) {  // slave may send less bytes than expected.
        uint8_t c = Wire.read();         // receive a byte as character
        cell->CurrentFlags[i] = c;
        i++;
    }
    if(i==0) {
        cell->IsOnline = false;
        // cell->PrintOut("I2c_commu::ReadSingleCell()  has no response");
        // Serial.println(" I2c_commu::ReadSingleCell()  No response.  cell_address= " + cell->Address);
    } 
    Wire.endTransmission(true);
    // delay(1000);
}

void I2c_commu::SpinOnce(){
       for (uint8_t i= 0; i< this->__CELLS_COUNT; i++){
        TouchCell* pCell = &(this->Cells[i]);
        if (pCell->IsOnline){
            pCell->PrintOut("Trying to read a cell");
            pCell->CopyCurrentToLast();
            this->ReadSingleCell(pCell);
            pCell->CompareCurrentAndLast();
            if (!pCell->IsOnline){
                Logger::Warn("I2c_commu::SpinOnce()  A cell is offline ");
                Serial.print("cell address = ");
                Serial.println(pCell->Address);
            }
        }
    }
    // bool all_is_offline = false;
    for(int i=0; i< this->__CELLS_COUNT; i++){
        if (this->Cells[i].IsOnline)
            return;
    }


    //All cells is offline. re init all cells to online
    for(int i=0; i< this->__CELLS_COUNT; i++){
        this->Cells[i].IsOnline = true;

    }
    Logger::Info("Reset all cells state to online");
    Serial.print("Min_address= ");
    Serial.print(this->Cells[0].Address);
    Serial.print("   CELLS_COUNT= ");
    Serial.print(this->__CELLS_COUNT);
    Serial.print(FCBC_RESET);
}

TouchCell* I2c_commu::FindandReadValidateCell(){
    for (uint8_t i= 0; i< this->__CELLS_COUNT; i++){
        TouchCell* pCell = &(this->Cells[i]);
        if (pCell->IsOnline){
            pCell->PrintOut("Trying to read a cell");
            pCell->CopyCurrentToLast();
            this->ReadSingleCell(pCell);
            pCell->CompareCurrentAndLast();
            if (pCell->IsOnline){
                return pCell;
            }
        }
    }
    //All cells is offline. re init all cells to online
    for(int i=0; i< this->__CELLS_COUNT; i++){
        this->Cells[i].IsOnline = true;
    }
    // }
    return nullptr;
}
