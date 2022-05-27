#include "track_sensor_base.h"


/*
If Cannot found, return -1
*/
void TrackSensorBase::__FindTrackFromRightEnd(uint8_t the_byte, uint8_t start_bit_from_right){
    bool last_bit = false;
    bool this_bit;
    this->_TrackStartBit = -1;
    this->_TrackWidth = 0;
    for (int bit=start_bit_from_right; bit<8; bit++){
        this_bit = (the_byte >> bit) & 0x01;
        if (this_bit){
            // Got a set bit
            this->_TrackWidth++;
            if (this->_TrackStartBit < 0){
                //Never got start_bit
                this->_TrackStartBit = bit;
            }
        }else if (last_bit){
            // last_bit == true, and  this_bit == false
            return ;
        }
        last_bit = this_bit;
    }
}


/*
bit7, bit16,  ...    bit1, bit0
left <--------------------> right
*/
int TrackSensorBase::_Convert_fromOrigin_toPositionError(uint8_t* origin_bits){
    // 1. Which track should I follow?
    // 2. How many tracks are there?
    // 3. Calculate the position error.  Base class doesn't know how to do this.
    this->__FindTrackFromRightEnd(origin_bits[0], 0);
    //backup right track information
    int8_t right_trackStartBit = this->_TrackStartBit;
    int8_t right_trackWidth = this->_TrackWidth; 
    if (!this->FollowRightTrack){
        uint8_t start_bit = this->_TrackStartBit + this->_TrackWidth;
        this->__FindTrackFromRightEnd(origin_bits[0], start_bit);
        if (this->_TrackStartBit == -1){
            //Can not find left track, copy right track back.
            this->_TrackStartBit = right_trackStartBit;
            this->_TrackWidth = right_trackWidth;
        }
    }
}
