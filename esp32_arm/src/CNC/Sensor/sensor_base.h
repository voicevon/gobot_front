#pragma once


class SensorBase{
    public:
        bool raw_value_is_absolute;
        virtual void SetCurrentPosition(float current_cnc_position);
        virtual float GetCurrentPosition();

    protected:
        virtual float _GetRawValue();

};