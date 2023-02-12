#include "board.h"

// http://www.bachinmaker.com/wikicn/doku.php?id=bachin-e3
//  x-end   GPIO 17   x-step GPIO-12  x-dir GPIO-14  
//  y-end   GPIO 4    y-step GPIO-26  y-dir GPIO-15
//                    z-step GPIO-27  z-dir GPIO-33





// Index number
#define PIN_POSITION_TRIGGER_X 4   //    7

#define PIN_ALPHA_DIR 14 //32  
#define PIN_ALPHA_STEP 12 //26   
#define PIN_ALPHA_ENABLE 13

// #define PIN_POSITION_TRIGGER_X  128
// #define PIN_POSITION_TRIGGER 1

#define PIN_LED_1 4
#define PIN_LED_2 11
#define PIN_LED_3 11
#define PIN_LED_4 11
#define PIN_LED_5 11
#define PIN_LED_6 11
#define PIN_LED_7 11


// // Index number
#define POSITION_TRIGGER_ALPHA 0

Twh_LoopPorter_Board::Twh_LoopPorter_Board(){
    _InitSerialBoard("Hello, I am Twh_LoopPorter_Board");
}

void Twh_LoopPorter_Board::Init(){
    #define POSITION_TRIGGER_COUNT 1

    __all_position_triggers[POSITION_TRIGGER_ALPHA].Init('X',PIN_POSITION_TRIGGER_X, LOW);
    PositionTrigger_Array::Instance().Init(__all_position_triggers, POSITION_TRIGGER_COUNT);

    __leds[0].Init(0, PIN_LED_1, LOW);
    __leds[1].Init(1, PIN_LED_2, LOW);
    __leds[2].Init(2, PIN_LED_3, LOW);
    __leds[3].Init(3, PIN_LED_4, LOW);
    __leds[4].Init(4, PIN_LED_5, LOW);
    __leds[5].Init(5, PIN_LED_6, LOW);
    __leds[6].Init(6, PIN_LED_7, LOW);

    __InitSteppers();

}

void Twh_LoopPorter_Board::__InitSteppers(){
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
        Logger::Info("Twh_LoopPorter_Board::Init()");
        Logger::Print("stepper alpha is OK.", 0);
    }else{
        Logger::Error("Twh_LoopPorter_Board::Init() ");
        Logger::Halt("failed FastAccelStepper.");
    }

}


void Twh_LoopPorter_Board::TurnOn_ThisLed_Only(int led_id){
    for(int i=0; i<7; i++){
        __leds[i].TurnOff();
        if (i==led_id) __leds[i].TurnOn();
    }
}

void Twh_LoopPorter_Board::Test_PositionTriggers(int loops){
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
            Serial.print("Trigger is channged   0:normal,   1:triggered \t\t");
            Serial.print("Counter= ");
            Serial.print(count);
            Serial.print("  ");
            Serial.println(flags, BIN);

            last_flags = flags;
            count++;
        }
    }
}

void Twh_LoopPorter_Board::Test_Stepper(int loops){
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

void Twh_LoopPorter_Board::Test_StepperQueue(int loops){
    // __stepper_alpha->queueEntries
}
