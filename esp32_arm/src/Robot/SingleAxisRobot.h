#ifndef _SINGLE_AXIS_ROBOT_H_
#define _SINGLE_AXIS_ROBOT_H_

#include "Axis/SingleAxisBase.hpp"
#include <Robot/Gcode.h>

template <class Actuator_T, class ActuatorController_T, class ActuatorDriver_T, class CommuDevice_T>
class SingleAxisRobot{
    public:
        SingleAxisRobot(char axisName);
        void RunGcode(Gcode* gcode);
        void LinkActuatorController(ActuatorController_T* actuatorController);
        // void LinkActuatorDriver(ActuatorDriver_T* actuatorDriver){this->actuatorDriver=actuatorDriver;};
        void LinkCommuDevice(CommuDevice_T* commuDevice){this->commuDevice=commuDevice;};
        void LinkAxis(SingleAxisBase<Actuator_T>* axis){this->singleAxis = axis;};
        virtual void Home();
        SingleAxisBase<Actuator_T>* singleAxis;
        // float GetCurrentPos(){return 0.0f};  //TODO: Read from actuator, like encoder, or Stepper, Servo.
    protected:
        SingleAxisRobot(){};
        ActuatorController_T* actuatorController;
        ActuatorDriver_T* actuatorDriver;
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


#endif