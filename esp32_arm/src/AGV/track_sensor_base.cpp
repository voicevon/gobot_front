#include "track_sensor_base.h"


/*
If Cannot found, return -1
*/
int TrackSensorBase::FindTrackFromRightEnd(uint8_t the_byte, uint8_t start_bit_from_right){
    for (int i=start_bit_from_right; i<8; i++){
        if((the_byte >> i ) & 0x01 == 1){
            return i;
        }
    }
    return -1;
}


/*
bit15, bit14,  ...    bit1, bit0
left <--------------------> right
*/
int TrackSensorBase::Convert_fromOrigin_toPositionError(uint8_t* origin_bits){
    int right_bit = 0;
    int left_bit = 0;
    int error = 0;
    // Find the first high bit , from right to left
    for(int i=0; i<2;i++){
        uint8_t the_byte = origin_bits[i];

    }
}
