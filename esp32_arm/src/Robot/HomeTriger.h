#ifndef _HOME_TRIGER_H_
#define _HOME_TRIGER_H_

#include <Arduino.h>
#include "MyLibs/Components/BinaryInput.h"
#include "Robot/Axis/SingleAxis.h"

class HomeTriger: public BinaryInput{
    public:
        HomeTriger(uint8_t pinTriger, uint8_t trigeredlState=LOW);
        // void SetHomePosition();   //???
        bool IsTriged();
        void LinkAxis(SingleAxis* axis){this->axis=axis;};
        void HomeAxis();
        float home_position;

    private:
        // uint8_t pinTriger;
        uint8_t trigeredState;
        SingleAxis* axis;
};


#endif