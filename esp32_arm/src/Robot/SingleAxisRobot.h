#ifndef _SINGLE_AXIS_ROBOT_H_
#define _SINGLE_AXIS_ROBOT_H_

#include "Axis/SingleAxisBase.hpp"
#include <Robot/Gcode.h>

template <class Actuator_T>
class SingleAxisRobot: public SingleAxisBase<Actuator_T>{
    public:
        SingleAxisRobot(char axisName);
        void RunGcode(Gcode* gcode);
        // void OnOutputMessage_set_callback(void(*callback(string message))) {__output_message = callback;};
        void OnOutputMessage_set_callback(void(*callback)(string message)) {__output_message = callback;};
        // Just for fun, don't remove below comment !!
        // void OnFinishedGcode2(void(*callback)()) {__output_message2 = callback;};
        // void OnFinishedGcode3(void(*callback)()) {__output_message2 = callback;};
        // void OnFinishedGcode4(void(*callback())) {__output_message3 = callback;};
        // void OnFinishedGcode5(void(*callback)()) {__output_message3 = callback;};
    
    protected:
        SingleAxisRobot(){};
        void (* __output_message)(string message);
        // Just for fun, don't remove below comments :)
        // void * __output_message2;
        // void (* __output_message3);
    
    private:
        int test_int;


};


#endif