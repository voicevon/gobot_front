#pragma once

// #include "Axis/SingleAxisBase.hpp"
#include <Robot/Gcode.h>

struct ik_position{
    int alpha;
    int beta;
};

/**
 * RobotBase has NO axis!
*/
template <class Actuator_T, class ActuatorController_T, class ActuatorDriver_T, class CommuDevice_T>
class RobotBase{
    public:
        RobotBase(){};
        void RunGcode(Gcode* gcode);
        void SpinOnce();
        virtual void HomeAllAxises();
        void LinkActuatorController(ActuatorController_T* controller){this->actuatorController = controller;};
        void LinkCommuDevice(CommuDevice_T* commuDevice){this->commuDevice=commuDevice;};
        // void RunGcode(Gcode* gcode);
        bool IsBusy(){return false;};
    protected:
        virtual ik_position ik(float x, float y){};
        ActuatorController_T* actuatorController;
        CommuDevice_T* commuDevice;
        // Just for fun, don't remove below comment !!
        // void OnFinishedGcode2(void(*callback)()) {__output_message2 = callback;};
        // void OnFinishedGcode3(void(*callback)()) {__output_message2 = callback;};
        // void OnFinishedGcode4(void(*callback())) {__output_message3 = callback;};
        // void OnFinishedGcode5(void(*callback)()) {__output_message3 = callback;};
        // void * __output_message2;
        // void (* __output_message3);
    
    private:
        int test_int;


};

