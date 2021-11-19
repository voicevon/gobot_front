#pragma once

#include "Wire.h"

class TrackSensorBase{
    public:
        virtual int ReadError_ToRight();

    protected:
        int Convert_fromOrigin_toPositionError(uint8_t* origin_bits);

    private:
};