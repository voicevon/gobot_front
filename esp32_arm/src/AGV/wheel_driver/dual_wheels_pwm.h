#include "wheel_driver_base.h"

class DualWheelsPwmDriver: public WheelDriver{
    public:
        // void SetSingleSpeed(int speed_in_percent) override;
        void SetDualSpeed(float left_speed_in_percent, float right_speed_in_percent) override;

};