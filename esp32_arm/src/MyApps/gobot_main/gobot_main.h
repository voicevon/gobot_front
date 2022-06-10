#pragma once

#include "actions.h"
#include "CNC/gcode/gcode_queue.h"
#include "chessboard_map.h"
#include "CNC/gcode/gcode_producer.h"
#include "IoT/mqtt_message_consumer.h"

class GobotMain: public GcodeProducer, public MqttMessageConsumer{
    public:
        GobotMain(){};
        // static GobotChessboard& getInstance()
        // {
        //     static GobotChessboard instance; // Guaranteed to be destroyed.
        //                           // Instantiated on first use.
        //     return instance;
        // }
        void SpinOnce();
        void ParkArms(bool do_home);
        void pick_place_park(RobotAction* pAction);
        void Calibrate(int step);

        void Test_HomeAlpha(int loop_count);
        void Test_HomeBeta(int loop_count);
        // void Test_EefUpDown(int loop_count);
        // void Test_EefSuckRelease(int loop_count);
    

    private:
        // RobotAction* __arm_action;
        void ExecuteMqttCommand(const char* command) override;

        void __Pickup(ChessboardCell* cell);
        void __Place(ChessboardCell* cell);
        void __Park();
        void __Home();
        void __Calibrate_99();
        String __GetGcode_for_eef_action(EefAction eef_action);
};