#pragma once

#include "actions.h"
#include "Robot/command_queue.h"

class GobotChessboard{
    public:
        static GobotChessboard& getInstance()
        {
            static GobotChessboard instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }
        void Init();
        void SpinOnce();
        void ParkArms(bool do_home);
        void pick_place_park(RobotAction* pAction);
        void Calibrate(int step,EefAction enable_eef_coil);

    private:
        GobotChessboard(){};
        RobotAction* __arm_action;
        CommandQueue* __commandQueue;
};