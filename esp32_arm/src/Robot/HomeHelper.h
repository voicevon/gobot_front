#ifndef _HOME_TRIGER_H_
#define _HOME_TRIGER_H_

#include <Arduino.h>
#include "Robot/Axis/SingleAxis.h"

class HomeHelper{
    public:
        HomeHelper(uint8_t pinTriger, int trigeredlState);
        // void SetHomePosition();   //???
        bool IsTriged();
        void LinkAxis(SingleAxis* axis){this->axis=axis;};
        // void HomeAxis();
        // float home_position;

    private:
        uint8_t pinTriger;
        int trigeredState;
        SingleAxis* axis;
};


#endif