#pragma once

#include "ASRS/sensor_jetty/jetty_sensor_base.h"
#include "MyBoards/board_base.h"
class BoardAsrs: public BoardBase{
    public:
        void Init(bool is_on_reset);
        JettySensorBase* GetJettySensor(){return &this->__jettySensor;};

    private:
        JettySensorBase __jettySensor;
};