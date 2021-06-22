#pragma once

#include "accel_stepper/AccelStepper.h"
#include "accel_stepper/MultiStepper.h"
#include "actions.h"
#include "ble_server.h"

// struct point_position{
//     int x;
//     int y;
// };
struct motor_position{
    int alpha;
    int beta;
};


// Up to 10 steppers can be handled as a group by MultiStepper

class Arm{
    public:
        Arm();
        void Home(unsigned char axis);
        void Init(void);
        void SpinOnce(ArmAction action);
        void pick_place_park(ArmAction arm_action);

    private:
        // home pin defination
        unsigned int alpha_home_pin = 23;
        unsigned int beta_home_pin = 24;
        // link length in mm
        int l0 = 4.05;   // Length between origin and the two motors
        int l1 = 8.05;   // Length from motor to passive joints
        int l2 = 12.05;  // Length from passive joints to end effector


        // EG X-Y position bed driven by 2 steppers
        // Alas its not possible to build an array of these with different pins for each :-(
        AccelStepper stepper1;
        AccelStepper stepper2;
        MultiStepper steppers;
        motor_position ik(int x, int y);
        BleServer* __ble_server;
    

    protected:

};
