#pragma once

// #include "Axis/SingleAxisBase.hpp"
#include <Robot/Gcode.h>
#include "Robot/Commu/CommuDeviceBase.h"

#include "Axis/SingleAxis.h"
// #include "Robot/Actuator/ActuatorControllerBase.h"
// #include "Robot/Actuator/ActuatorBase.h"
// struct ik_position{
//     int alpha;
//     int beta;
// };



class FkPositionBase{

};
class FkPosX: public FkPositionBase{
    public:
        float x;
};
class FkPosXY: public FkPositionBase{
    public:
        float x;
        float y;
};
class FkPosXYZ: public FkPositionBase{
    public:
        float x;
        float y;
        float z;
};

class fkPosXYZA: public FkPositionBase{
    public:
        float x;
        float y;
        float z;
        float a;
};

class fkPosXYZAB: public FkPositionBase{
    public:
        float x;
        float y;
        float z;
        float a;
        float b;
};
class fkPosXYZABC: public FkPositionBase{
    public:
        float x;
        float y;
        float z;
        float a;
        float b;
        float c;
};

class fkPosXYUVW: public FkPositionBase{
    public:
        float x;
        float y;
        float z;
        float u;
        float v;
        float w;
};

class IkPositionBase{

};
class IkPosX: public IkPositionBase{
    public:
        float alpha;
};
class IkPosXY: public IkPositionBase{
    public:
        float alpha;
        float beta;
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
        void RunGcode(Gcode* gcode);
        void SpinOnce();
        virtual void Init_Linkage();
        virtual void HomeAllAxises();   //??
        
        bool IsIdle(){return this->robot_is_idle;};
        // virtual void Init();

    protected:
        RobotBase(){};
        virtual bool MoveToTargetPosition();
        virtual void SpinOnce_BaseEnter();
        virtual void SpinOnce_BaseExit();
        void LinkCommuDevice(CommuDeviceBase* commuDevice){this->commuDevice=commuDevice;};
        virtual IkPositionBase* IK(FkPositionBase* fk);
        virtual FkPositionBase* FK(IkPositionBase* ik);
        virtual void RunG1(Gcode* gcode);
        virtual const char* GetHomeTrigerStateString();
        // virtual void Move(Gcode* gcode);   //??
        CommuDeviceBase* commuDevice;
        // FkPositionBase* currentFkPosition;  //??
        // FkPositionBase* nextFkPosition;   //??
        bool robot_is_idle = true;    //TODO: Will be instead of commu_buffer_length > 0.
        bool is_absolute_position = true;
        // Just for fun, don't remove below comment !!
        // void OnFinishedGcode2(void(*callback)()) {__output_message2 = callback;};
        // void OnFinishedGcode3(void(*callback)()) {__output_message2 = callback;};
        // void OnFinishedGcode4(void(*callback())) {__output_message3 = callback;};
        // void OnFinishedGcode5(void(*callback)()) {__output_message3 = callback;};
        // void * __output_message2;
        // void (* __output_message3);
    
    private:
        int test_int;
        void _base_spin_once();



};

