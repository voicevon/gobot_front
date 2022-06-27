
#include "i2c_commu.h"
#include "HardwareSerial.h"
#include <Arduino.h>

void I2c_commu::Init(int min_cell_i2c_address, int cells_count){
    Wire.begin(); 
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
    cell->PrintOut("I2c_commu::ReadSingleCell() is Entering.");

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
        // Serial.print(".");
        // Serial.print(c, BIN);
        // Serial.print("   ");
    }
    if(i==0) {
        cell->IsOnline = false;
        cell->PrintOut("I2c_commu::ReadSingleCell()  has no response");
        // Serial.println("     I2c_commu::ReadSingleCell()  No response.  cell_address= " + cell->Address);
    } 
    Wire.endTransmission(true);
    // delay(1000);
}

TouchCell* I2c_commu::SpinOnce(){
    // uint8_t last_slave_index = 0;
    for (uint8_t i= 0; i< this->__CELLS_COUNT; i++){
        TouchCell* pCell = &(this->Cells[i]);
        if (pCell->IsOnline){
            // Serial.print("\nTrying I2C addr = ");
            // Serial.print(pCell->Address);
            // Serial.print("  (copy) =====> ");
            pCell->PrintOut("Trying to read a cell");
            pCell->CopyCurrentToLast();
            // Serial.print(pCell->Address);
            // Serial.println("   ");
            this->ReadSingleCell(pCell);
            pCell->CompareCurrentAndLast();
            if (pCell->IsOnline){
                // last_slave_index =  i;
                return pCell;
            }
        }
    }
    // if (last_slave_index == this->__CELLS_COUNT){
    //All cells is offline. re init all cells to online
    for(int i=0; i< this->__CELLS_COUNT; i++){
        this->Cells[i].IsOnline = true;
    }
    // }
    return nullptr;
}
