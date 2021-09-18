#ifndef _SINGLE_AXIS_ROBOT_H_
#define _SINGLE_AXIS_ROBOT_H_

#include "Axis/SingleAxisBase.hpp"
#include <Robot/Gcode.h>

template <class Actuator_T>
class SingleAxisRobot: public SingleAxisBase<Actuator_T>{
    public:
        SingleAxisRobot(char axisName);
        void RunGcode(Gcode* gcode);
        void OnFinishedGcode(void(*callback(string message))) {__on_finished_gcode = callback;};
        // Just for fun, don't remove below comment !!
        // void OnFinishedGcode2(void(*callback)()) {__on_finished_gcode2 = callback;};
        // void OnFinishedGcode3(void(*callback)()) {__on_finished_gcode2 = callback;};
        // void OnFinishedGcode4(void(*callback())) {__on_finished_gcode3 = callback;};
        // void OnFinishedGcode5(void(*callback)()) {__on_finished_gcode3 = callback;};
    
    protected:
        SingleAxisRobot(){};
    
    private:
        void (* __on_finished_gcode)(string message);
        // Just for fun, don't remove below comments :)
        // void * __on_finished_gcode2;
        // void (* __on_finished_gcode3);


};


#endif