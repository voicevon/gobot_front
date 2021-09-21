#ifndef __H_TASK_BASE_H_
#define __H_TASK_BASE_H_

#include <Arduino.h>

/*
HTask is High performanceTask
*/
class HTaskBase{
    public:
        HTaskBase();
        void SpinOnce(){};
        bool IsBusy;

        void StartDelay(int us){};

    protected:
        uint32_t _timer_started_at;
};



#endif