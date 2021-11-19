#include "track_sensor_i2c.h"
#include "HardwareSerial.h"


TrackSensor_I2C::TrackSensor_I2C(){
    Wire.begin();
}

int TrackSensor_I2C::ReadError_ToRight(){
    Serial.print("ReadError_ToRight()");
    uint8_t n_bytes = 1;
    Wire.beginTransmission(slave_address);
    Wire.endTransmission(false);
    Wire.requestFrom(slave_address, n_bytes);    // request data from slave device
    int i=0;
    while (Wire.available() > 0) {  // slave may send less than requested
        uint8_t c = Wire.read();         // receive a byte as character
        RxBuffer[0] = c;
        i++;
        Serial.println(c,BIN);
    }
    Wire.endTransmission(true);
    // delay(1000);

    int error = RxBuffer[0];
    return error;
}