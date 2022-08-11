#pragma once

#include "board_vsc.h"
// #include "CNC/Actuator/dc_motor/actuator_dc_motor.h"
#include "MyBoards/board_test/board_test_cnc.h"

class Vsc_BoardTest: public BoardTestCnc{
    public:
        // void LinkBoard(CncBoardBase* board) override;
        void Test_AngleSensor(MagneticSensorAnalog* sensor,int loop_count);
        void Test_MotorDriver(ActuatorDcMotor* motor, int loop_count);
        void Test_Actuator(ActuatorDcMotor* motor, int loop_count);
        void Test_Offset(int loop_count);
        // void Test_AllHomers(int loop_count) override;


    private:
        // Vsc_Board* __board;
        void __TestOffset(float value);
};