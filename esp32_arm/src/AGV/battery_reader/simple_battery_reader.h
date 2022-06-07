#pragma once
#include "battery_reader_base.h"

class SimpleBatteryReader: public BatteryReaderBase{
    public:
        float ReadVolt() override;

    private:

};
