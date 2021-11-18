#include "track_sensor_i2c.h"
#include "HardwareSerial.h"


TrackSensor_I2C::TrackSensor_I2C(){
    Wire.begin();
}

int TrackSensor_I2C::ReadError_ToRight(){
    // void I2c_commu::ReadSingleSlave(uint8_t slave_address){

    Serial.print("    ");
    Serial.print(slave_address);
    // uint8_t slave_addr = cell->Address;
    uint8_t n_bytes = 1;
    // cell->Address = 3;
    Wire.beginTransmission(slave_address);
    Wire.endTransmission(false);
    Wire.requestFrom(slave_address, n_bytes);    // request data from slave device
    int i=0;
    while (Wire.available() > 0) {  // slave may send less than requested
        uint8_t c = Wire.read();         // receive a byte as character
        RxBuffer[0] = c;
        i++;
        Serial.print(".");
        // Serial.print(c, BIN);
        // Serial.print("   ");
    }
    Wire.endTransmission(true);
    // delay(1000);

    int error = RxBuffer[0];
    return error;
}