#pragma once
#include "stdint.h"

class TouchPad_Channel{
    public:
        enum EnumState{
            NOT_INSTALLED,
            WROKING,
            CELL_DIED,
            CHANNEL_DIED,
        };
        void Init(uint8_t cell_id, uint8_t channel_id, EnumState state);

    private:
        uint8_t __cell_id;
        uint8_t __channel_id;
        EnumState __state;
        

};