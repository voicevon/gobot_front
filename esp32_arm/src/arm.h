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

// #include "accel_stepper/AccelStepper.h"
// #include "accel_stepper/MultiStepper.h"
#include "mcp23018.h"
#include "actions.h"
#include "ESP32Step/src/TeensyStep.h"

// #include "ble_server.h"
#include <ESP32Servo.h>


// struct point_position{
//     int x;
//     int y;
// };
struct motor_position{
    int alpha;
    int beta;
};


// Up to 10 steppers can be handled as a group by MultiStepper

enum EEF{
    Lower = 1,
    Higher = 2,
    Suck = 3,
    Release = 4,
    Sleep = 5
};

class Arm{
    public:
        static Arm& getInstance()
        {
            static Arm instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }
        void Home(unsigned char axis);
        void SpinOnce(void);
        void Setup(RobotAction* pAction);
        // void pick_place_park(BodyAction* body_action);
        void MoveTo(int x, int y);
        void SetEffector(EEF action);
        void pick_place_park(RobotAction* pAction);

    private:
        Arm();
        Arm(Arm const& copy);            // Not Implemented
        Arm& operator=(Arm const& copy); // Not Implemented

        motor_position ik(int x, int y);

        // link length in mm
        int l0 ;   // Length between origin and the two motors
        int l1 ;   // Length from motor to passive joints
        int l2 ;  // Length from passive joints to end effector


        // EG X-Y position bed driven by 2 steppers
        // Alas its not possible to build an array of these with different pins for each :-(
        // AccelStepper stepper_alpha(AccelStepper::MotorInterfaceType::FULL4WIRE, 6, 7, 8, 9,true);
        int STEPS_PER_RAD;
        // AccelStepper* stepper_alpha;
        // AccelStepper* stepper_beta;
        // MultiStepper steppers;
        Stepper* stepper_alpha;
        Stepper* stepper_beta;
        StepControl steppers;

        Servo* eefServo;

        uint8_t __home_pin;
        bool homed = false;
        // AccelStepper* stepper;
        uint8_t homing_axis;

        RobotAction* __arm_action;

        void HomeSpin(void);

    protected:

};
