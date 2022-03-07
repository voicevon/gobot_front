#pragma once
/*
    This AGV is driven by 2 wheels. 
        each wheel is driven by L298N
    Track sensor is 8 bit in front of veheical frame.
        The bot connect sensor with I2C bus.
*/
#include "AGV/vehical_base.h"
#include "AGV/track_sensor_shengteng.h"
#include "Robot/Actuator/DCMotor/h_bridge_l298n.h"
#include "Robot/Actuator/DCMotor/DCMotorController.h"

#include "Robot/RobotBase.h"

#define PIN_LEFT_WHEEL_DC_MOTOR_A 12
#define PIN_LEFT_WHEEL_DC_MOTOR_B 14
#define PIN_LEFT_WHEEL_DC_MOTOR_ENABLE 25
#define PIN_RIGHT_WHEEL_DC_MOTOR_A 26
#define PIN_RIGHT_WHEEL_DC_MOTOR_B 27
#define PIN_RIGHT_WHEEL_DC_MOTOR_ENABLE 33

#define PWM_CHANNEL_0 0
#define PWM_CHANNEL_1 1
// #define PWM_CHANNEL_2 2
// #define PWM_CHANNEL_3 3


/*
G1 Y?? F1   Forward slow moving 
G1 Y?? F9   Forward fast moving 
M301 P1 I1 D1    //Set PID
*/

class TwinWheelHardware: public VehicalBase, public RobotBase{

    public:
        void Init() override;
        void LinkPid(PIDController* wheel_pid);
        void MoveForward(int track_error) override;
        void Stop() override;
   

        ActuatorBase leftWheel = ActuatorBase();
        ActuatorBase rightWheel = ActuatorBase();


        void InitRobot() override{};
        void HomeSingleAxis(char axis) override{};
        void RunG1(Gcode* gcode) override{};

        bool GetCurrentPosition(FkPositionBase* position_fk) override{return false;};  
        float GetDistanceToTarget_FK() override{return 0.0;};
        float GetDistanceToTarget_IK() override{};


    protected:

    private:
        L298N objLeftWheelBridge = L298N();
        L298N objRightWheelBridge = L298N();

       //Override private
        void SpinOnce_BaseEnter() override {};
        void SpinOnce_BaseExit() override {};
        virtual void IK(FkPositionBase* from_fk,IkPositionBase* to_ik) override {};
        virtual void FK(IkPositionBase* ik, FkPositionBase*  to_fk) override{};

        void RunG6(Gcode* gcode) override{} ; 
        void _running_G1() override {};
        void _running_G28() override{};
        void RunM123(uint8_t eef_channel, EefAction eef_action) override{};
        void RunM84() override{};
        std::string GetHomeTrigerStateString() override {return " ";};
        void __EnableMotor(char actuator, bool enable_it) override{};

};


