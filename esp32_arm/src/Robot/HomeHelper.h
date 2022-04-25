#ifndef _HOME_TRIGER_H_
#define _HOME_TRIGER_H_

#include <Arduino.h>

class HomeHelper{
    public:
        // For OC output sensor, trigeredState should be LOW
        HomeHelper(uint8_t pinTriger, int trigeredState);
        // void SetHomePosition();   //???
        bool IsTriged();
        void SetTrigeredCountMax(int new_value){this->trigered_counter_max = new_value;};

    private:
        int trigered_counter = 0;
        int trigered_counter_max = 3;
        uint8_t pinTriger;
        int trigeredState;
};


#endif