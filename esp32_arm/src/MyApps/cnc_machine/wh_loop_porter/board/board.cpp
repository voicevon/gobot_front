#include "board.h"


#define PIN_ALPHA_DIR 27 //32  
#define PIN_ALPHA_STEP 14 //26   
#define PIN_ALPHA_ENABLE 32

#define PIN_LED_1 23
#define PIN_LED_2 22
#define PIN_LED_3 21
#define PIN_LED_4 19
#define PIN_LED_5 18
#define PIN_LED_6 17
#define PIN_LED_7 16

#define PIN_NUMBER_LED_SCLK 25
#define PIN_NUMBER_LED_DATA 26
#define PIN_NUMBER_LED_LOAD 33

#define PIN_POSITION_TRIGGER_X 12   //  Hall sensor:   OH44E (南京欧卓) 

Twh_LoopPorter_Board::Twh_LoopPorter_Board(){
    _InitSerialBoard("Hello, I am Twh_LoopPorter_Board");
}

void Twh_LoopPorter_Board::Init(){
    #define POSITION_TRIGGER_COUNT 1

    __all_position_triggers[0].Init('X',PIN_POSITION_TRIGGER_X, LOW);
    PositionTrigger_Array::Instance().Init(__all_position_triggers, POSITION_TRIGGER_COUNT);

    __leds[0].Init(0, PIN_LED_1, LOW);
    __leds[1].Init(1, PIN_LED_2, LOW);
    __leds[2].Init(2, PIN_LED_3, LOW);
    __leds[3].Init(3, PIN_LED_4, LOW);
    __leds[4].Init(4, PIN_LED_5, LOW);
    __leds[5].Init(5, PIN_LED_6, LOW);
    __leds[6].Init(6, PIN_LED_7, LOW);

    __InitSteppers();
    __displayer.Init(PIN_NUMBER_LED_SCLK, PIN_NUMBER_LED_DATA, PIN_NUMBER_LED_LOAD, 2);
    __displayer.ShowNumber(0);

}

void Twh_LoopPorter_Board::__InitSteppers(){
    __stepper_engine.init(1);

    __stepper_alpha = __stepper_engine.stepperConnectToPin(PIN_ALPHA_STEP);

    if (__stepper_alpha) {
        __stepper_alpha->setDirectionPin(PIN_ALPHA_DIR, true, 0);   
        __stepper_alpha->setEnablePin(PIN_ALPHA_ENABLE, true);        //Low is active enable.                    
        __stepper_alpha->setAutoEnable(false);
        // __stepper_alpha->setSpeedInUs(6);  // the parameter is us/step !!!
        __stepper_alpha->setSpeedInHz( 7 * 1000 );
        __stepper_alpha->setAcceleration(3 * 1000);
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

// void Twh_LoopPorter_Board::Test_PositionTriggers(int loops){
//     #define POSITION_TRIGGER_COUNT 1
//     uint32_t flags = 0;
//     uint32_t last_flags = 999;
//     int count =0;
//     while (count < loops){
//         flags = 0;
//         for (int index=0; index < POSITION_TRIGGER_COUNT; index++){
//             if (__all_position_triggers[index].IsFired()){
//                 flags += 1<<index;
//             }
//         }
//         if (flags != last_flags){
//             Serial.print("Trigger is channged   0:normal,   1:triggered \t\t");
//             Serial.print("Counter= ");
//             Serial.print(count);
//             Serial.print("  ");
//             Serial.println(flags, BIN);

//             last_flags = flags;
//             count++;
//         }
//     }
// }

void Twh_LoopPorter_Board::Test_Stepper(int loops){
    FastAccelStepper* stepper= __stepper_alpha;
    stepper->setAcceleration(3000);
    stepper->setSpeedInHz(7*1000);
    stepper->enableOutputs();
    for (int i=0; i<loops; i++){
        Logger::Print("Test stepper loop======================================", i);
        if (stepper) {
            // 5 circles.
            stepper->setSpeedInHz(14*1000);
            stepper->moveTo(5500999, false);
            while (stepper->isRunning()){
                Logger::Print("Current position", stepper->getCurrentPosition());
                delay(300);
            }
        stepper->setSpeedInHz(7*1000);
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
