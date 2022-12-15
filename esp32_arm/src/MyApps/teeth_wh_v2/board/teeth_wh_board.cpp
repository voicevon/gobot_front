#include "teeth_wh_board.h"

// http://www.bachinmaker.com/wikicn/doku.php?id=bachin-e3

#define PIN_HOMER_SENSOR_HALL_ALPHA  17 //23
#define PIN_HOMER_SENSOR_HALL_X  4 //16
// #define PIN_VACUUME_PUMP 0 // 33

// #define PIN_SERVO_EEF_VERTICAL 18   
// #define PIN_SERVO_VACUUM_SWITCH 19  

#define PIN_IR_CHECKING 32

// #define PIN_HX711_CLK 22
// #define PIN_HX711_DATA 22

#define PIN_VL6180_XXX 22
#define PIN_VL6180_XXX 22
#define PIN_VL6180_XXX 22

// #define PIN_ALPHA_DIR 14 //32  
#define PIN_ALPHA_STEP 12 //26   
// #define PIN_BETA_DIR  15 //14  
#define PIN_BETA_STEP  26 //12  
// #define PIN_STEPPER_ENABLE 13

#define PIN_POSITION_TRIGGER_X  128
#define PIN_POSITION_TRIGGER_Z  5
#define PIN_POSITION_TRIGGER_ROW_0 129
#define PIN_POSITION_TRIGGER_ROW_1 129
#define PIN_POSITION_TRIGGER_ROW_2 129
#define PIN_POSITION_TRIGGER_ROW_3 129
#define PIN_POSITION_TRIGGER_ROW_4 129
#define PIN_POSITION_TRIGGER_ROW_5 129
#define PIN_POSITION_TRIGGER_ROW_6 129

#define PIN_STEPPER_DIR_ROW_0  133
#define PIN_STEPPER_DIR_ROW_1  133
#define PIN_STEPPER_DIR_ROW_2  133
#define PIN_STEPPER_DIR_ROW_3  133
#define PIN_STEPPER_DIR_ROW_4  133
#define PIN_STEPPER_DIR_ROW_5  133
#define PIN_STEPPER_DIR_ROW_6  133

#define PIN_STEPPER_DIR_ALPHA  144
#define PIN_STEPPER_DIR_BETA   144

#define PIN_STEPPERS_ENABLE 155

// Index number
#define POSITION_TRIGGER_ALPHA 0
#define POSITION_TRIGGER_X  1
// #define SERVO_VACUUM_SWITCH  0
// #define SERVO_EEF_VERTICAL 1


void Twh2_Board::Init(bool is_on_reset){
    if (is_on_reset){
        Serial.begin(115200);
        Serial.println("I am Teeth Warehouse.");
    }

    pinMode(PIN_IR_CHECKING, INPUT_PULLUP);

    __all_position_triggers[POSITION_TRIGGER_ALPHA].Init('Y',PIN_HOMER_SENSOR_HALL_ALPHA, HIGH);
    __all_position_triggers[POSITION_TRIGGER_X].Init('a', PIN_HOMER_SENSOR_HALL_X, HIGH);

    __InitSteppers();

    // Init Hx711
    // __hx711.begin(PIN_HX711_DATA, PIN_HX711_CLK);

    // Init VL6180
    // Wire.begin();         // Start I2C library
    // delay(100);           // delay .1s



    // if (sensor.VL6180xInit() != 0)
    // {
    //     Serial.println("Failed to initialize. Freezing..."); // Initialize device and check for errors
    //     while (1);
    // }

    // sensor.VL6180xDefautSettings(); // Load default settings to get started.

    // delay(1000); // delay 1s

}

void Twh2_Board::__InitSteppers(){
    __stepper_engine.init(1);
    __stepper_alpha = __stepper_engine.stepperConnectToPin(PIN_ALPHA_STEP);  // for alpha=27, for beta=12
    __stepper_beta = __stepper_engine.stepperConnectToPin(PIN_BETA_STEP);  // for alpha=27, for beta=12

    if (__stepper_alpha) {
        __stepper_alpha->setDirectionPin(PIN_STEPPER_DIR_ALPHA);   //for alpha=26, for beta = 14
        __stepper_alpha->setEnablePin(PIN_STEPPERS_ENABLE);                            
        __stepper_alpha->setAutoEnable(false);

        __stepper_alpha->setSpeedInUs(1000);  // the parameter is us/step !!!
        __stepper_alpha->setAcceleration(100);
        // int res =  __stepper_alpha->moveTo(-1000, false);
        // Logger::Print("moveTo() returns", res);
        Logger::Info("Twh2_Board::Init()");
        Logger::Print("stepper alpha is OK.", 0);
    }else{
        Logger::Error("Twh2_Board::Init() ");
        Logger::Halt("failed FastAccelStepper.");
    }

    if (__stepper_beta) {
        __stepper_beta->setDirectionPin(PIN_STEPPER_DIR_BETA);   //for alpha=26, for beta = 14
        __stepper_beta->setEnablePin(PIN_STEPPERS_ENABLE);                          
        __stepper_beta->setAutoEnable(false);

        __stepper_beta->setSpeedInUs(1000);  // the parameter is us/step !!!
        __stepper_beta->setAcceleration(100);
        Logger::Info("Twh2_Board::Init()");
        Logger::Print("stepper alpha is OK.", 0);
    }else{
        Logger::Error("Twh2_Board::Init() ");
        Logger::Halt("failed FastAccelStepper.");
    }
}

void Twh2_Board::Test_PositionTriggers(int loops){
    uint32_t flags = 0;
    uint32_t last_flags = 999;
    int count =0;
    while (count < loops){
        flags = 0;
        for (int index=0; index < HOME_TRIGGER_COUNT; index++){
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

void Twh2_Board::Test_SingleStepper(int index, int loops){
    FastAccelStepper* stepper;
    if (index==0)  stepper = __stepper_alpha;
    if (index==1)  stepper = __stepper_beta;
    stepper->enableOutputs();
    for (int i=0; i<loops; i++){
        Logger::Print("Test stepper loop=", i);
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
        // Stepper* motor;
        // Serial.println("aaaaaaaaaa");
        // if (index == 0) motor = &__stepper_alpha;
        // if (index == 1) motor = &__stepper_beta;
        // Serial.println("bbbbbbbbbbbbbbbb");

        // for (int i=0; i<loops; i++){
        //     Serial.println("cccccccccccccc");
        //     __stepper_alpha.setTargetRel(1000);  // Set target position to 1000 steps from current position
        //     Serial.println("ffffffffff");
        //     controller->move(__stepper_alpha);    // Do the move
        //     Serial.println("hhhhh");
        //     delay(500);

        //     // motor->setTargetRel(-1000);  // Set target position to 1000 steps from current position
        //     // controller->move(*motor);    // Do the move
        //     // delay(500);
        // }
        
}

void Twh2_Board::Test_DualStepper(int loops){
    __stepper_alpha->enableOutputs();
    for (int i=0; i<loops; i++){
        Logger::Info("Test_DualStepper");
        Logger::Print("loop", i);
        // Move to Y+
        __stepper_alpha->moveTo(1000, false);
        __stepper_beta->moveTo(-1000,false);
        while (__stepper_alpha->isRunning()){
            // Logger::Print("alpha position", __stepper_alpha->getCurrentPosition());
            // Logger::Print("beta position", __stepper_beta->getCurrentPosition());
            delay(300);
        }
        // Move to Y-.
        __stepper_alpha->moveTo(-1000, false);
        __stepper_beta->moveTo(1000, false);
        while (__stepper_beta->isRunning()){
            // Logger::Print("alpha position", __stepper_alpha->getCurrentPosition());
            // Logger::Print("beta position", __stepper_beta->getCurrentPosition());
            delay(300);
        }
    }
    __stepper_alpha->disableOutputs();
}
void Twh2_Board::Test_VL6180x(){
    // VL6180xIdentification identification;
    // sensor.getIdentification(&identification); // Retrieve manufacture info from device memory
    // // printIdentification(&identification);      // Helper function to print all the Module information
    // Serial.print("Model ID = ");
    // Serial.println(identification->idModel);

    // Serial.print("Model Rev = ");
    // Serial.print(identification->idModelRevMajor);
    // Serial.print(".");
    // Serial.println(identification->idModelRevMinor);

    // Serial.print("Module Rev = ");
    // Serial.print(identification->idModuleRevMajor);
    // Serial.print(".");
    // Serial.println(identification->idModuleRevMinor);

    // Serial.print("Manufacture Date = ");
    // Serial.print((identification->idDate >> 3) & 0x001F);
    // Serial.print("/");
    // Serial.print((identification->idDate >> 8) & 0x000F);
    // Serial.print("/1");
    // Serial.print((identification->idDate >> 12) & 0x000F);
    // Serial.print(" Phase: ");
    // Serial.println(identification->idDate & 0x0007);

    // Serial.print("Manufacture Time (s)= ");
    // Serial.println(identification->idTime * 2);
    // Serial.println();
    // Serial.println();
}
