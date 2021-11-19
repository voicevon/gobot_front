#pragma once

#include "Wire.h"

class TrackSensorBase{
    public:
        virtual int ReadError_ToRight();
        bool FollowRightTrack = true;

    protected:
        int _Convert_fromOrigin_toPositionError(uint8_t* origin_bits);
        int8_t _TrackStartBit;
        int8_t _TrackWidth;   //Normaly is 1 or 2.

    private:
        void __FindTrackFromRightEnd(uint8_t the_byte, uint8_t start_bit_from_right);
};