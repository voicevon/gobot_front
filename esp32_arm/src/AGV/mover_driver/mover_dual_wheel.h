#pragma once

#include "mover_base.h"
#include "single_wheel_driver/single_wheel_driver_base.h"

class MoverDualWheel: public MoverBase{
    public:
        MoverDualWheel();
        void LinkLeftDriver(SingleWheelDriverBase* leftwheelDriver){this->__leftDriver=leftwheelDriver;};
        void LinkRightDriver(SingleWheelDriverBase* rightWheelDriver){this->__rightDriver=rightWheelDriver;};
        void SetForwdingSpeed(uint8_t forwarding_speed_in_percent, float angle) override;

    protected:
        SingleWheelDriverBase* __leftDriver;
        SingleWheelDriverBase* __rightDriver;

    private:


};