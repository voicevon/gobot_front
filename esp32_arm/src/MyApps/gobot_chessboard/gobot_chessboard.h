#pragma once

#include "actions.h"
#include "Robot/gcode_queue.h"
#include "chessboard_map.h"
#include "Robot/gcode_producer.h"

class GobotChessboard: public GcodeProducer{
    public:
        static GobotChessboard& getInstance()
        {
            static GobotChessboard instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }
        // void Init(GcodeQueue* gcode_queue);
        void SpinOnce();
        void ParkArms(bool do_home);
        void pick_place_park(RobotAction* pAction);
        void Calibrate(int step);
        // GcodeQueue* GetCommandQueue(){return this->__commandQueue;};

    private:
        GobotChessboard(){};
        RobotAction* __arm_action;
        // GcodeQueue* __commandQueue;
        void __Pickup(ChessboardCell* cell);
        void __Place(ChessboardCell* cell);
        void __Park();
        void __Home();
        void __Calibrate_99();
        String __GetGcode_for_eef_action(EefAction eef_action);
};