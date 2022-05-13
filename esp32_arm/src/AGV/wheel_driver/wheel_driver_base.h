# pragma once


class WheelDriver{
    public:
        // virtual void SetSingleSpeed(int speed_in_percent);
        virtual void SetDualSpeed(float left_speed_in_percent, float right_speed_in_percent);
};