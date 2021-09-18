#ifndef _SINGLE_AXIS_H_
#define _SINGLE_AXIS_H_

#include "SingleAxisBase.hpp"
#include <Robot/Gcode.h>

template <class Actuator_T>
class SingleAxis: public SingleAxisBase<Actuator_T>{
    public:
        SingleAxis(char axisName);
        void RunGcode(Gcode* gcode);
        void OnFinishedGcode(void(*callback(string message))) {__on_finished_gcode = callback;};
        // Just for fun, don't remove below comment !!
        // void OnFinishedGcode2(void(*callback)()) {__on_finished_gcode2 = callback;};
        // void OnFinishedGcode3(void(*callback)()) {__on_finished_gcode2 = callback;};
        // void OnFinishedGcode4(void(*callback())) {__on_finished_gcode3 = callback;};
        // void OnFinishedGcode5(void(*callback)()) {__on_finished_gcode3 = callback;};
    
    
    private:
        void (* __on_finished_gcode)(string message);
        // Just for fun, don't remove below comments :)
        // void * __on_finished_gcode2;
        // void (* __on_finished_gcode3);


};


#endif