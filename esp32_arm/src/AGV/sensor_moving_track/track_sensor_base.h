#pragma once

#include "Wire.h"

class TrackSensorBase{
    public:
        bool FollowRightTrack = true;
        virtual int16_t ReadAlignmentError();
        // virtual int ReadError_FromRight(uint8_t* rxBuffer){};
        virtual bool GetFlag_Slowdown(){return true;};
        virtual bool GetFlag_Speedup(){return true;};
        virtual void ClearFlag_Slowdown(){};
        virtual void TurnOnLed(bool turn_on){};
        virtual int16_t ReadForwardingError(){return 0;};
        virtual void ReadParkingError(int16_t* x_error, int16_t* y_error){};
        bool IsFollowingLeft;

    protected:
        int _Convert_fromOrigin_toPositionError(uint8_t* origin_bits);
        int8_t _TrackStartBit;
        int8_t _TrackWidth;   //Normaly is 1 or 2.

    private:
        void __FindTrackFromRightEnd(uint8_t the_byte, uint8_t start_bit_from_right);
};