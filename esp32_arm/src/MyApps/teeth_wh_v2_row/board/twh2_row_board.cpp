#include "twh2_row_board.h"

// http://www.bachinmaker.com/wikicn/doku.php?id=bachin-e3




// Index number
#define PIN_POSITION_TRIGGER_X 17

#define PIN_ALPHA_DIR 14 //32  
#define PIN_ALPHA_STEP 12 //26   
#define PIN_ALPHA_ENABLE 13

// #define PIN_POSITION_TRIGGER_X  128
// #define PIN_POSITION_TRIGGER 1


// // Index number
#define POSITION_TRIGGER_ALPHA 0

Twh2Row_Board::Twh2Row_Board(){
    _InitSerialBoard("Hello, I am Twh2Row_Board");
}

void Twh2Row_Board::Init(){
    #define POSITION_TRIGGER_COUNT 1

    __all_position_triggers[POSITION_TRIGGER_ALPHA].Init('X',PIN_POSITION_TRIGGER_X, HIGH);
    PositionTrigger_Array::Instance().Init(__all_position_triggers, POSITION_TRIGGER_COUNT);

    __InitSteppers();

}

void Twh2Row_Board::__InitSteppers(){
    __stepper_engine.init(1);

    __stepper_alpha = __stepper_engine.stepperConnectToPin(PIN_ALPHA_STEP);

    if (__stepper_alpha) {
        __stepper_alpha->setDirectionPin(PIN_ALPHA_DIR, true, 0);   
        __stepper_alpha->setEnablePin(PIN_ALPHA_ENABLE, true);        //Low is active enable.                    
        __stepper_alpha->setAutoEnable(false);
        __stepper_alpha->setSpeedInUs(1000);  // the parameter is us/step !!!
        __stepper_alpha->setAcceleration(100);
        // int res =  __stepper_alpha->moveTo(-1000, false);
        // Logger::Print("moveTo() returns", res);
        Logger::Info("Twh2Row_Board::Init()");
        Logger::Print("stepper alpha is OK.", 0);
    }else{
        Logger::Error("Twh2Row_Board::Init() ");
        Logger::Halt("failed FastAccelStepper.");
    }

}

void Twh2Row_Board::Test_PositionTriggers(int loops){
    #define POSITION_TRIGGER_COUNT 1
    uint32_t flags = 0;
    uint32_t last_flags = 999;
    int count =0;
    while (count < loops){
        flags = 0;
        for (int index=0; index < POSITION_TRIGGER_COUNT; index++){
            if (__all_position_triggers[index].IsFired()){
                flags += 1<<index;
            }
        }
        if (flags != last_flags){
            Serial.print("Trigger is channged: \t\t");
            Serial.print("Counter= ");
            Serial.print(count);
            Serial.print("  ");
            Serial.println(flags,BIN);

            last_flags = flags;
            count++;
        }
    }
}

void Twh2Row_Board::Test_Stepper(int loops){
    FastAccelStepper* stepper= __stepper_alpha;
    stepper->enableOutputs();
    for (int i=0; i<loops; i++){
        Logger::Print("Test stepper loop======================================", i);
        if (stepper) {
            // 5 circles.
            stepper->moveTo(360.0f /1.8f * 16 * 1, false);
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

void Twh2Row_Board::Test_StepperQueue(int loops){
    // __stepper_alpha->queueEntries
}
