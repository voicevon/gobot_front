#pragma once

#include "actions.h"
#include "Robot/command_queue.h"
#include "chessboard_map.h"

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
        void Calibrate(int step);

    private:
        GobotChessboard(){};
        RobotAction* __arm_action;
        CommandQueue* __commandQueue;
        void __Pickup(ChessboardCell* cell);
        void __Place(ChessboardCell* cell);
        void __Park();
        String __GetGcode_for_eef_action(EefAction eef_action);
};