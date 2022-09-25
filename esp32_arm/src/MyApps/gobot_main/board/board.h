#pragma once
#include "CNC/board/cnc_board_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "board_pins/board_ver1.2.h"
#include "robot_eef/gobot_main_eef.h"
#include "CNC/solution/axis_homer/axis_homer.h"

class GobotMain_Board: public CncBoardBase{
    public:
        GobotMain_Board(){};
        void Init(bool is_on_reset) override;
        void PrintOut();
        Stepper* GetStepper(EnumAxis axis);
        RobotEef_GobotMain* GetEef() override;
        void EnableMotor(EnumAxis axis_name, bool enable_it) override;
        uint8_t ReadAllRooms();
        uint8_t GetLoadedRoom();


    private:
        void InitHardware();
    
        Stepper __alpha_stepper = Stepper(PIN_ALPHA_STEP_2201, PIN_ALPHA_DIR_2201);
        Stepper __beta_stepper = Stepper(PIN_BETA_STEP_2201, PIN_BETA_DIR_2201);

        PositionTrigger homer_alpha = PositionTrigger(PIN_HOME_ALPHA_2201, LOW);
        PositionTrigger homer_beta = PositionTrigger(PIN_HOME_BETA_2201, LOW);
        RobotEef_GobotMain __eef;

        uint8_t PIN_ROOMS[8];
        // static uint8_t PIN_ROOMS[] = {PIN_SENSOR_ROOM_0,
        //                 PIN_SENSOR_ROOM_1,
        //                 PIN_SENSOR_ROOM_2,
        //                 PIN_SENSOR_ROOM_3,
        //                 PIN_SENSOR_ROOM_4,
        //                 PIN_SENSOR_ROOM_5,
        //                 PIN_SENSOR_ROOM_6,
        //                 PIN_SENSOR_ROOM_7
        //                 };

};
