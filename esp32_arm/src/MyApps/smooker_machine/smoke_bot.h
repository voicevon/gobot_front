// #pragma once

// // #include "CNC/robot_hardware_base.h"
// #include "CNC/cnc_base.h"
// #include "ESP32Step/src/TeensyStep.h"
// #include "CNC/Actuator/ActuatorBase.h"
// // #include "CNC/HomeHelper.h"
// #include "CNC/single_axis_homer.h"
// // #include "CNC/Commu/CommuUart.h"

// #define PIN_HOME 18
// #define PIN_STEP 33
// #define PIN_DIR 32
// #define PIN_ENABLE 14
// #define MICRO_STEP_1 27
// #define MICRO_STEP_2 26
// #define MICRO_STEP_3 25



// class SmokeBotHardware: public CncBase{
//     public:
//         SmokeBotHardware();
//         void RunG28(char axis) override;
//         void InitRobot(BoardbaseCnc* board) override;
//         void Init_Gpio();
//         bool GetCurrentPosition(FkPositionBase* position_fk) override {return false;};
//         float GetDistanceToTarget_FK() override{return 0.0;};
//         float GetDistanceToTarget_IK() override{return 0.0;};

//     private:
//         // CommuUart objCommuUart = CommuUart();
//         Stepper objStepper = Stepper(PIN_STEP, PIN_DIR);
//         StepControl objStepControl;

//         SingleAxisHomer objHomeHelper = SingleAxisHomer(PIN_HOME, HIGH);
        
//         // void SpinOnce_BaseEnter() override {};
//         // void SpinOnce_BaseExit() override {};
//         void IK(FkPositionBase* from_fk, IkPositionBase* to_ik) override {};
//         void FK(IkPositionBase* from_ik, FkPositionBase* to_fk) override {};
//         void RunG1(Gcode* gcode) override;   //None blocking, move backgroundly.
//         void RunG6(Gcode* gcode) override {};   //Block mode
//         std::string GetHomeTrigerStateString() override {return " ";};

//         void _running_G1() override;
//         void _running_G28() override;
//         void RunM123(uint8_t eef_channel, EefAction eef_action) override {};
//         void RunM84() override{};
//         // void __EnableMotor(char actuator, bool enable_it) override;        


// };