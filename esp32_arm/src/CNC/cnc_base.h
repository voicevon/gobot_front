#pragma once

#include "CNC/eef_standard_code.h"
#include "CNC/Commu/CommuDeviceBase.h"
#include "CNC/gcode_consumer.h"

class FkPositionBase{
    
};
class FkPosition_X: public FkPositionBase{
    public:
        float x;
};

class FkPosition_XY: public FkPositionBase{
    public:
        float X;
        float Y;
};

class FkPosition_XYZ: public FkPositionBase{
    public:
        float x;
        float y;
        float z;
};

class FkPosition_XYZA: public FkPositionBase{
    public:
        float x;
        float y;
        float z;
        float a;
};

class FkPosition_XYZAB: public FkPositionBase{
    public:
        float x;
        float y;
        float z;
        float a;
        float b;
};

class FPosition_XYZABC: public FkPositionBase{
    public:
        float x;
        float y;
        float z;
        float a;
        float b;
        float c;
};

class FkPosition_ZW: public FkPositionBase{
    public:
        float Z;
        float W;
};

class FkPosition_YZ: public FkPositionBase{
    public:
        float Y;
        float Z;
};

class FkPosition_A: public FkPositionBase{
    public:
        float A;
};

class IkPositionBase{

};

// class FkPosition_AB: public FkPositionBase{
//     public:
//         float alpha;
//         float beta;
// };

class IkPosition_A: public IkPositionBase{
    public:
        float alpha;
};
class IkPosition_AB: public IkPositionBase{
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

enum class CncState{
    IDLE,
    RUNNING_G1,
    RUNNING_G4,
    RUNNING_G28
};
class CncBase: public GcodeConsumer{
    public:
        CncState State = CncState::IDLE;
        void RunGcode(Gcode* gcode);
        void SpinOnce();
        virtual void InitRobot();
        virtual void HomeSingleAxis(char axis);
        virtual bool GetCurrentPosition(FkPositionBase* position_fk);
        virtual float GetDistanceToTarget_FK();
        virtual float GetDistanceToTarget_IK();
        void SayHello();

    protected:
        // RobotBase(){};
        // void LinkCommuDevice(CommuDeviceBase* commuDevice){this->commuDevice=commuDevice;};
        // virtual void SpinOnce_BaseEnter();
        void SpinOnce_BaseExit();
        virtual void IK(FkPositionBase* from_fk, IkPositionBase* to_ik);
        virtual void FK(IkPositionBase* from_ik,FkPositionBase* to_fk);
        virtual void RunG1(Gcode* gcode);   //None blocking, move backgroundly.
        virtual void RunG6(Gcode* gcode);   //Block mode
        void RunG4(Gcode* gcode);
        virtual std::string GetHomeTrigerStateString();
        void RunM42(uint8_t pin_number, uint8_t pin_value);
        virtual void RunM84();
        virtual void RunM123(uint8_t eef_channel, EefAction eef_action);
        // TODO:  Should I remove this? 
        //  This commuDevice will only output message , for debugging perpose, Right?
        //  If is right, Then, HardSerial is sufficient to do that.
        // CommuDeviceBase* commuDevice;     
        bool is_absolute_position = true;

        /* Just for fun, don't remove below comment.
        void * __output_message2;
        void (* __output_message3); 
        void OnFinishedGcode2(void(*callback)()) {__output_message2 = callback;};
        void OnFinishedGcode3(void(*callback)()) {__output_message2 = callback;};
        void OnFinishedGcode4(void(*callback())) {__output_message3 = callback;};
        void OnFinishedGcode5(void(*callback)()) {__output_message3 = callback;};
        */

        virtual void _running_G1();
        virtual void _running_G28();
        char _homing_axis;
        bool _home_as_inverse_kinematic;   //When home sensor is trigered, What is the current position? Can use predefined FK position, also can use predefined IK position.
        virtual void __EnableMotor(char actuator, bool enable_it);
    
    private:
        int test_int;
        void _base_spin_once();
        void __running_G4();
        long __g4_start_timestamp;
        int __g4_time_second;



};

