#ifndef _HOME_TRIGER_H_
#define _HOME_TRIGER_H_

#include <Arduino.h>
// #include "Robot/Axis/SingleAxis.h"
#include "Robot/Actuator/ActuatorBase.h"

class HomeHelper{
    public:
        HomeHelper(uint8_t pinTriger, int trigeredlState);
        // void SetHomePosition();   //???
        bool IsTriged();
        // void LinkAxis(SingleAxis* axis){this->axis=axis;};
        void LinkActuator(ActuatorBase* actuator){this->actuator=actuator;};
        // void HomeAxis();
        // float home_position;

    private:
        uint8_t pinTriger;
        int trigeredState;
        // SingleAxis* axis;
        ActuatorBase* actuator;
};


#endif