#pragma once
// #include "MyBoards/const.h"


class ActuatorRangeConstraintBase{
    public:
        void InitCncRange(float min, float max){this->_cnc_range_min=min;this->_cnc_range_max=max;};
        void InitActuatorRange(float min, float max){this->_actuator_range_min=min;this->_actuator_range_max=max;};
        float _ConvertTo_ActuatorRange(float cnc_position) ;
        float _ConvertTo_CncRange(float actuator_position) ;

    protected:
        float _cnc_range_min;
        float _cnc_range_max;
        float _actuator_range_min = -99999;
        float _actuator_range_max = 99999;

    private:


};