#pragma once
#include "../board_base.h"
#include "ESP32Step/src/TeensyStep.h"
// #include "ESP32Step/src/Stepper.h"
#include "CNC/single_axis_homer.h"
// #include "gobot_house_ver3.11.h"
#include "board_gobot_house_ver3.11.h"

class CncBoard_AB_AlphaBeta: public BoardbaseCnc{
    public:
        CncBoard_AB_AlphaBeta(){};
        void Init() override;
        void SayHello();
        Stepper* GetStepper(char axis_name) override;
        SingleAxisHomer* GetHomer(char axis_name) override;
        void EnableMotor(char axis_name, bool enable_it) override;
        
    private:
        Stepper stepper_alpha = Stepper(PIN_ALPHA_STEP_2109, PIN_ALPHA_DIR_2109);
        Stepper stepper_beta = Stepper(PIN_BETA_STEP_2109,PIN_BETA_DIR_2109);
        SingleAxisHomer homer_alpha = SingleAxisHomer(PIN_HOME_ALHPA_2109, LOW);
        SingleAxisHomer homer_beta = SingleAxisHomer(PIN_HOME_BETA_2109, LOW);

};
