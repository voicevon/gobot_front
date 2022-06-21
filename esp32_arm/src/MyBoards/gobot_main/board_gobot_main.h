#pragma once
#include "MyBoards/cnc_board_base.h"
#include "ESP32Step/src/TeensyStep.h"
// #include "board_ver1.2.h"
#include "board_pins/board_ver1.2.h"
#include "robot_eef/gobot_main_eef.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"
#include "CNC/single_axis_homer.h"
#include "CNC/cnc_mover/mover_dual_stepper.h"

class Board_GobotMain: public CncBoardBase{
    public:
        Board_GobotMain(){};
        void Init(bool is_on_reset) override;
        ActuatorStepper* GetActuator(char axis_name) override;
        SingleAxisHomer* GetHomer(char axis_name) override;
        RobotEef_GobotMain* GetEef() override;
        void EnableMotor(char axis_name, bool enable_it) override;
        uint8_t ReadAllRooms();
        uint8_t GetLoadedRoom();


    private:
        void InitHardware();
        Stepper __stepper_alpha = Stepper(PIN_ALPHA_STEP_2201, PIN_ALPHA_DIR_2201);
        Stepper __stepper_beta = Stepper(PIN_BETA_STEP_2201, PIN_BETA_DIR_2201);
        ActuatorStepper __actuator_alpha;
        ActuatorStepper __actuator_beta;
        CncMover_DualStepper __mover_dual_step;

        SingleAxisHomer homer_alpha = SingleAxisHomer(PIN_HOME_ALPHA_2201, LOW);
        SingleAxisHomer homer_beta = SingleAxisHomer(PIN_HOME_BETA_2201, LOW);
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
