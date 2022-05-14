# pragma once


class MoverBase{
    public:
        // virtual void SetSingleSpeed(int speed_in_percent);
        virtual void SetForwdingSpeed(float forwarding_speed_in_percent, float angle);
};