#pragma once

#include "CNC/gcode/gcode_queue.h"
#include "CNC/gcode/gcode_producer.h"
#include "IoT/mqtt_message_consumer.h"
#include "Robot/eef/eef_standard_code.h"
#include "chessboard_map.h"

class GobotMain_Robot: public GcodeProducer, public MqttMessageConsumer{
    public:
        GobotMain_Robot(){};

        void SpinOnce();
        void ParkArms(bool do_home);
        void Calibrate(int step);
        void Test_HomeAlpha(int loop_count);
        void Test_HomeBeta(int loop_count);
        void Test_PickPlace(int loop_count);
    

    private:
        void AsyncExecuteMqttCommand(const char* command) override;

        void __Pickup(ChessboardCell* cell);
        void __Place(ChessboardCell* cell);
        void __Park();
        void __Home();
        void __Calibrate_99();
        String __GetGcode_for_eef_action(EefAction eef_action);
};