#include "board.h"

// #include "MyLibs/board/pins_yorahone.h"
#include "pin_dev.h"

#define MAX_SPEED 600
#define ACCCELLERATION 900


// // Index number
// #define POSITION_TRIGGER_ALPHA 0

Silicon_Pump_Board::Silicon_Pump_Board(){
    _InitSerialBoard("Hello, I am Silicon_Pump_Board");
}

void Silicon_Pump_Board::Init(){
    // #define POSITION_TRIGGER_COUNT 1

    // __all_position_triggers[POSITION_TRIGGER_ALPHA].Init('X',PIN_POSITION_TRIGGER_X, LOW);
    // PositionTrigger_Array::Instance().Init(__all_position_triggers, POSITION_TRIGGER_COUNT);
    __state_led.Init(0, PIN_STATE_LED, LOW);
    __InitSteppers();

}

void Silicon_Pump_Board::__InitSteppers(){
    __stepper_engine.init(1);

    __stepper_alpha = __stepper_engine.stepperConnectToPin(PIN_ALPHA_STEP);

    if (__stepper_alpha) {
        __stepper_alpha->setDirectionPin(PIN_ALPHA_DIR, true, 0);   
        __stepper_alpha->setEnablePin(PIN_ALPHA_ENABLE, true);        //Low is active enable.                    
        __stepper_alpha->setAutoEnable(false);
        __stepper_alpha->setSpeedInUs(MAX_SPEED);  // the parameter is us/step !!!
        __stepper_alpha->setAcceleration(ACCCELLERATION);
        // int res =  __stepper_alpha->moveTo(-1000, false);
        // Logger::Print("moveTo() returns", res);
        Logger::Info("Silicon_Pump_Board::Init()");
        Logger::Print("stepper alpha is OK.", 0);
    }else{
        Logger::Error("Silicon_Pump_Board::Init() ");
        Logger::Halt("failed FastAccelStepper.  alpha");
    }

    __stepper_beta = __stepper_engine.stepperConnectToPin(PIN_BETA_STEP);
    if (__stepper_beta) {
        __stepper_beta->setDirectionPin(PIN_BETA_DIR, true, 0);   
        __stepper_beta->setEnablePin(PIN_BETA_ENABLE, true);        //Low is active enable.                    
        __stepper_beta->setAutoEnable(false);
        __stepper_beta->setSpeedInUs(MAX_SPEED);  // the parameter is us/step !!!
        __stepper_beta->setAcceleration(ACCCELLERATION);
        // int res =  __stepper_alpha->moveTo(-1000, false);
        // Logger::Print("moveTo() returns", res);
        Logger::Info("Silicon_Pump_Board::Init()");
        Logger::Print("stepper beta is OK.", 0);
    }else{
        Logger::Error("Silicon_Pump_Board::Init() ");
        Logger::Halt("failed FastAccelStepper.   beta");
    }

    __stepper_gama = __stepper_engine.stepperConnectToPin(PIN_GAMMA_STEP);
    if (__stepper_gama) {
        __stepper_gama->setDirectionPin(PIN_GAMMA_DIR, false, 0);   
        __stepper_gama->setEnablePin(PIN_GAMMA_ENABLE, true);        //Low is active enable.                    
        __stepper_gama->setAutoEnable(false);
        __stepper_gama->setSpeedInUs(MAX_SPEED );  // the parameter is us/step !!!
        __stepper_gama->setAcceleration(ACCCELLERATION);
        // int res =  __stepper_alpha->moveTo(-1000, false);
        // Logger::Print("moveTo() returns", res);
        Logger::Info("Silicon_Pump_Board::Init()");
        Logger::Print("stepper gamma is OK.", 0);
    }else{
        Logger::Error("Silicon_Pump_Board::Init() ");
        Logger::Halt("failed FastAccelStepper.   gamma");
    }
}

FastAccelStepper* Silicon_Pump_Board::GetStepper(EnumAxis_Inverseinematic axis){
    if (axis == EnumAxis_Inverseinematic::AXIS_ALPHA){
        return __stepper_alpha;
    }else if (axis == EnumAxis_Inverseinematic::AXIS_BETA){
        return __stepper_beta;
    }else if (axis == EnumAxis_Inverseinematic::AXIS_GAMMA){
        return __stepper_gama;
    }else{
        Logger::Error("Silicon_Pump_Board::GetStepper()   Unkonwn axis");
        Logger::Halt(" I can not sleep. ");
    }
}



void Silicon_Pump_Board::Test_Stepper(int loops){
    FastAccelStepper* stepper= __stepper_alpha;
    stepper->setAcceleration(600);
    stepper->setSpeedInHz(8000);
    stepper->enableOutputs();
    for (int i=0; i<loops; i++){
        Logger::Print("Test stepper loop======================================", i);
        if (stepper) {
            // 5 circles.
            stepper->moveTo(360.0f /1.8f * 16 * 5.8 * 100, false);
            while (stepper->isRunning()){
                Logger::Print("Current position", stepper->getCurrentPosition());
                delay(300);
            }
            stepper->moveTo(0, false);
            while (stepper->isRunning()){
                Logger::Print("Current position", stepper->getCurrentPosition());
                delay(300);
            }
        }
    }
    stepper->disableOutputs();
        
}

void Silicon_Pump_Board::Test_StepperQueue(int loops){
    // __stepper_alpha->queueEntries
}
