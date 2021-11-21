#include "track_sensor_shengteng.h"
#include "HardwareSerial.h"




int TrackSensor_Shengteng::ReadError_FromRight(uint8_t* rxBuffer){


    this->_Convert_fromOrigin_toPositionError(rxBuffer);
    if (this->_TrackStartBit== -1) return 999; // Not found black mark
    if (this->_TrackWidth == 8) return 888;     //Whole black
    
    int error=0;
    if (this->_TrackStartBit == 0 && this->_TrackWidth == 1) error = 10;
    else if (this->_TrackStartBit == 0 && this->_TrackWidth == 1) error = 10;
    else if (this->_TrackStartBit == 0 && this->_TrackWidth == 2) error = 8;
    else if (this->_TrackStartBit == 1 && this->_TrackWidth == 1) error = 6;
    else if (this->_TrackStartBit == 1 && this->_TrackWidth == 2) error = 4;
    else if (this->_TrackStartBit == 2 && this->_TrackWidth == 1) error = 3;
    else if (this->_TrackStartBit == 2 && this->_TrackWidth == 2) error = 2;
    else if (this->_TrackStartBit == 3 && this->_TrackWidth == 1) error = 1;
    else if (this->_TrackStartBit == 3 && this->_TrackWidth == 2) error = 0;
    else if (this->_TrackStartBit == 4 && this->_TrackWidth == 1) error = -1;
    else if (this->_TrackStartBit == 4 && this->_TrackWidth == 2) error = -2;
    else if (this->_TrackStartBit == 5 && this->_TrackWidth == 1) error = -3;
    else if (this->_TrackStartBit == 5 && this->_TrackWidth == 2) error = -4;
    else if (this->_TrackStartBit == 6 && this->_TrackWidth == 1) error = -6;
    else if (this->_TrackStartBit == 6 && this->_TrackWidth == 2) error = -8;
    else if (this->_TrackStartBit == 7 && this->_TrackWidth == 1) error = -10;
    else{
        Serial.print("\n\n [ERROR] TrackSensor_I2C::ReadError_ToRight()     ");
        Serial.print(*rxBuffer);
        Serial.print("   ");
        Serial.print(this->_TrackStartBit);
        Serial.print("   ");
        Serial.print(this->_TrackWidth);
    }
    return error;
}