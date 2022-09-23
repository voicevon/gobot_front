#pragma once
#include "MyBoards/cnc_board_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "board_pins/board_ver1.2.h"
#include "robot_eef/gobot_main_eef.h"
#include "CNC/Sensor/position_trigger/position_triggers.h"

class GobotMain_Board: public CncBoardBase{
    public:
        GobotMain_Board(){};
        void Init(bool is_on_reset) override;
        void PrintOut();
        // void LinkStepControlToCncMover(StepControl* stepControl){this->__mover_dual_step.LinkStepControl(stepControl);};
        // ActuatorBase* GetActuator(EnumAxis axis_name) override;
        Stepper* GetStepper(EnumAxis axis);
        SinglePositionTriger* GetSingleHomer(EnumAxis axis_name) override;
        PositionTriggers* GetPositionTriggers(EnumAxis axis_name) override;
        RobotEef_GobotMain* GetEef() override;
        // CncSolutionConfigBase* GetCncConfig() override {return &this->__cnc_solution_config;};
        void EnableMotor(EnumAxis axis_name, bool enable_it) override;
        uint8_t ReadAllRooms();
        uint8_t GetLoadedRoom();


    private:
        void InitHardware();
        PidControllers* GetPidControllers() override {Logger::Error("GobotMain_Board::GetPidControllers()");};
    
        Stepper __alpha_stepper = Stepper(PIN_ALPHA_STEP_2201, PIN_ALPHA_DIR_2201);
        Stepper __beta_stepper = Stepper(PIN_BETA_STEP_2201, PIN_BETA_DIR_2201);
        // ActuatorRangeConstraintBase __alpha_range;
        
        // ActuatorStepper __actuator_alpha;
        // ActuatorStepper __actuator_beta;
        // ActuatorRangeConstraintBase __beta_range;
        // CncMover_DualStepper __mover_dual_step;

        SinglePositionTriger homer_alpha = SinglePositionTriger(PIN_HOME_ALPHA_2201, LOW);
        SinglePositionTriger homer_beta = SinglePositionTriger(PIN_HOME_BETA_2201, LOW);
        RobotEef_GobotMain __eef;

        // GobotMain_CncSolutionConfig __cnc_solution_config;
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
