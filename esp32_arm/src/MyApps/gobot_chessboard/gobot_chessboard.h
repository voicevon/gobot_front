#pragma once
/*
Stepper liberys:

    https://github.com/gin66/FastAccelStepper
    https://github.com/luni64/TeensyStep
       * Does't support ESP32
    - https://github.com/RCP1/ESP32Step
    https://github.com/Stan-Reifel/FlexyStepper
    - https://github.com/pkerspe/ESP-FlexyStepper
*/

#define PIN_LED_POWER_2112 12
#define PIN_LED_B_2112 25
#define PIN_LED_C_2112 26
#define PIN_LED_F_2112 27
#define PIN_LED_H_2112 14

#define PIN_HOME_ALHPA_2112  35     //??
#define PIN_ALPHA_DIR_2112 19
#define PIN_ALPHA_STEP_2112 5

#define PIN_HOME_BETA_2112  34      //??
#define PIN_BETA_DIR_2112 17
#define PIN_BETA_STEP_2112 4


#define PIN_ENDER_COIL_2112 32
#define PIN_ENDER_COIL_EXT_2112 33



#include "actions.h"
#include <ESP32Servo.h>
#include "Robot/RobotBase.h"
#include "ESP32Step/src/TeensyStep.h"
#include "Robot/Commu/CommuBleGattServer.h"
#include "Robot/Commu/CommuUart.h"
#include "MyLibs/MyFunctions.hpp"
#include "Robot/Gcode.h"



// #include "ESP32Step/src/TeensyStep.h"
#include "Robot/HomeHelper.h"
#include "MyLibs/Components/Led.h"
// Up to 10 steppers can be handled as a group by MultiStepper

enum EEF{
    Lower = 1,
    Higher = 2,
    Suck = 3,
    Release = 4,
    Sleep = 5
};


/*
What I know:    
        X,Y position.

What I don't know:
        Chessboard size, and where is chessboard. 
        Stone point position,
        Trashbin position,

How to solve the concepts I don't know?
    Please send X,Y position to me.
    Don't send me nominal name of go game (either of any other name).

*/
class GobotChessboard: public RobotBase{
    public:
        static GobotChessboard& getInstance()
        {
            static GobotChessboard instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }
        void HomeAllAxises() override;
        void RunG1(Gcode* gcode) override;
        void Init_Linkage() override;
        // void SpinOnce(void);
        void Setup(RobotAction* pAction);

        void SetEffector(EEF action);
        void pick_place_park(RobotAction* pAction);
    
    private:
        GobotChessboard();
        GobotChessboard(GobotChessboard const& copy);            // Not Implemented
        GobotChessboard& operator=(GobotChessboard const& copy); // Not Implemented

        // ik_position ik(float x, float y) override;
        virtual IkPositionBase* IK(FkPositionBase* fk) override;
        virtual FkPositionBase* FK(IkPositionBase* ik) override;

        // SingleAxisBase<Stepper> obj_axis_alpha = SingleAxisBase<Stepper>('A');
        // SingleAxisBase<Stepper> obj_axis_beta = SingleAxisBase<Stepper>('B');

        // link length in mm
        int link_0 ;   // Length between origin and the two motors
        int link_a ;   // Length from motor to passive joints
        int link_b ;  // Length from passive joints to end effector


        // EG X-Y position bed driven by 2 steppers
        // Alas its not possible to build an array of these with different pins for each :-(
        // AccelStepper stepper_alpha(AccelStepper::MotorInterfaceType::FULL4WIRE, 6, 7, 8, 9,true);
        int STEPS_PER_RAD;
        // MultiStepper steppers;


        Servo* eefServo;

        void __HomeSpin(Stepper* homing_stepper, uint8_t home_pin);
        bool homed;

        RobotAction* __arm_action;


    protected:
        void SpinOnce_BaseEnter() override {};
        void SpinOnce_BaseExit() override {};
    private:
        Led objLedPower = Led(0, PIN_LED_POWER_2112, LOW);
        Led objLedHome_alpha = Led(1,2,LOW);
        HomeHelper objHomeHelper_alpha = HomeHelper(PIN_HOME_ALHPA_2112, HIGH);
        HomeHelper objHomeHelper_beta = HomeHelper(PIN_HOME_BETA_2112, HIGH);

        // SingleAxis objAxis_Alpha = SingleAxis();
        // SingleAxis objAxis_Beta = SingleAxis();
        ActuatorBase objActuator_Alpha = ActuatorBase();
        ActuatorBase objActuator_Beta = ActuatorBase();

        Stepper objStepper_alpha = Stepper(PIN_ALPHA_STEP_2112, PIN_ALPHA_DIR_2112);
        Stepper objStepper_beta = Stepper(PIN_BETA_STEP_2112, PIN_BETA_DIR_2112);
        StepControl objStepControl;

        CommuBleGattServer objCommuBle = CommuBleGattServer();
        CommuUart objCommuUart = CommuUart();
        IkPosXY objIkPos;
        FkPosXY objFkPos;
};


