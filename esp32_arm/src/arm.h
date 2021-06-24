#pragma once

#include "accel_stepper/AccelStepper.h"
#include "accel_stepper/MultiStepper.h"
#include "mcp23018.h"
#include "actions.h"

#include "ble_server.h"
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
        void SpinOnce(ArmAction action);
        void pick_place_park(ArmAction arm_action);

    private:
        Arm();
        // Arm(Arm const& copy);            // Not Implemented
        // Arm& operator=(Arm const& copy); // Not Implemented

        motor_position ik(int x, int y);
        void MoveTo(int x, int y);
        void SetEffector(EEF action);
        void pick_place_park(ArmAction* arm_action);

        // link length in mm
        int l0 ;   // Length between origin and the two motors
        int l1 ;   // Length from motor to passive joints
        int l2 ;  // Length from passive joints to end effector


        // EG X-Y position bed driven by 2 steppers
        // Alas its not possible to build an array of these with different pins for each :-(
        // AccelStepper stepper_alpha(AccelStepper::MotorInterfaceType::FULL4WIRE, 6, 7, 8, 9,true);
        int STEPS_PER_RAD;
        AccelStepper* stepper_alpha;
        AccelStepper* stepper_beta;
        MultiStepper* steppers;
        Servo* eefServo;
        BleServer* __ble_server;
        // Mcp23018* __Mcp23018;

    protected:

};
