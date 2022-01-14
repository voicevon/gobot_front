#ifndef _HOME_TRIGER_H_
#define _HOME_TRIGER_H_

#include <Arduino.h>

class HomeHelper{
    public:
        HomeHelper(uint8_t pinTriger, int trigeredlState);
        // void SetHomePosition();   //???
        bool IsTriged();

    private:
        uint8_t pinTriger;
        int trigeredState;
};


#endif