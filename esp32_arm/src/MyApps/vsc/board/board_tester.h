#pragma once

#include "board_vsc.h"
// #include "CNC/board_test/cnc_board_test.h"
#include "CNC/board/cnc_board_test.h"


class Vsc_BoardTest: public CncBoard_Test{
    public:
        // void LinkBoard(CncBoardBase* board) override;
        void Test_AngleSensor(MagneticSensorAnalog* sensor,int loop_count);
        void Test_EncoderSensor(Encoder* sensor, int loop_count);
        void Test_MotorDriver(ActuatorDcMotor* motor, int loop_count);
        void Test_Actuator(ActuatorDcMotor* motor, int loop_count);
        void Test_Offset(int loop_count);
        // void Test_AllHomers(int loop_count) override;


    private:
        // Vsc_Board* __board;
        void __TestOffset(float value);
};