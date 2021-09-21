#pragma once

// #include "Axis/SingleAxisBase.hpp"
#include <Robot/Gcode.h>
#include "Robot/Commu/CommuDeviceBase.h"

#include "Axis/SingleAxis.h"
// #include "Robot/Actuator/ActuatorControllerBase.h"
// #include "Robot/Actuator/ActuatorBase.h"
struct ik_position{
    int alpha;
    int beta;
};

/**
 * RobotBase has NO axis! 
 *      Reasons:
 *          1. Doesn't know how may axis. 
 *      ?? Or has at least one axis? 
 *      ?? Saying has no actuator, driver, sensor ?
 * RobotBase has NO ActuatorController!
 *          The ActuatorController might be inside of ActuatorDriver.
 *                                 might be no ActuatorCotroller in the whole system.
*/
class RobotBase{
    public:
        RobotBase(){};
        void RunGcode(Gcode* gcode);
        virtual void SpinOnce();
        virtual void HomeAllAxises();   //??
        virtual void Init();
        bool IsBusy(){return this->is_busy;};
        // void AppendAxis(SingleAxis* axis);
    protected:
        void LinkCommuDevice(CommuDeviceBase* commuDevice){this->commuDevice=commuDevice;};
        virtual ik_position ik(float x, float y);
        virtual void RunG1(Gcode* gcode);
        void _base_spin_once();
        CommuDeviceBase* commuDevice;
        bool is_busy = false;

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

