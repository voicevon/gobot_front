#include "teeth_wh_board.h"

// http://www.bachinmaker.com/wikicn/doku.php?id=bachin-e3

#define PIN_HOMER_SENSOR_HALL_ALPHA  17 //23
#define PIN_HOMER_SENSOR_HALL_X  4 //16
#define PIN_VACUUME_PUMP 0 // 33

#define PIN_SERVO_EEF_VERTICAL 18   
#define PIN_SERVO_VACUUM_SWITCH 19  

#define PIN_IR_CHECKING 32

#define PIN_HX711_CLK 22
#define PIN_HX711_DATA 22

#define PIN_VL6180_XXX 22
#define PIN_VL6180_XXX 22
#define PIN_VL6180_XXX 22

#define PIN_ALPHA_DIR 14 //32  
#define PIN_ALPHA_STEP 12 //26   
#define PIN_BETA_DIR  15 //14  
#define PIN_BETA_STEP  26 //12  
#define PIN_STEPPER_ENABLE 13


// Index number
#define POSITION_TRIGGER_ALPHA 0
#define POSITION_TRIGGER_X  1
#define SERVO_VACUUM_SWITCH  0
#define SERVO_EEF_VERTICAL 1


void TeethWarehouse_Board::Init(bool is_on_reset){
    if (is_on_reset){
        Serial.begin(115200);
        Serial.println("I am Teeth Warehouse.");
    }

    pinMode(PIN_IR_CHECKING, INPUT_PULLUP);

    __all_position_triggers[POSITION_TRIGGER_ALPHA].Init('Y',PIN_HOMER_SENSOR_HALL_ALPHA, HIGH);
    __all_position_triggers[POSITION_TRIGGER_X].Init('a', PIN_HOMER_SENSOR_HALL_X, HIGH);
    // __all_position_triggers[2].Init(PIN_HOMER_SENSOR_HALL_2, LOW);
    PositionTrigger_Array::Instance().Init(__all_position_triggers, HOME_TRIGGER_COUNT);
    
    // Disable vacuumPump
    pinMode(PIN_VACUUME_PUMP, OUTPUT);
    this->EnableVacuumPump(false);


    // Init servo.
    ESP32PWM::allocateTimer(0);   //https://github.com/madhephaestus/ESP32Servo/blob/master/examples/Multiple-Servo-Example-Arduino/Multiple-Servo-Example-Arduino.ino
    __all_servos[SERVO_EEF_VERTICAL].setPeriodHertz(50);  // Standard 50hz servo
    __all_servos[SERVO_EEF_VERTICAL].attach(PIN_SERVO_EEF_VERTICAL);
    __all_servos[SERVO_EEF_VERTICAL].write(260);
    
    __all_servos[SERVO_VACUUM_SWITCH].setPeriodHertz(50);  // Standard 50hz servo
    __all_servos[SERVO_VACUUM_SWITCH].attach(PIN_SERVO_VACUUM_SWITCH);

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

void TeethWarehouse_Board::__InitSteppers(){
    __stepper_engine.init(1);
    __stepper_alpha = __stepper_engine.stepperConnectToPin(PIN_ALPHA_STEP);  // for alpha=27, for beta=12
    __stepper_beta = __stepper_engine.stepperConnectToPin(PIN_BETA_STEP);  // for alpha=27, for beta=12

    if (__stepper_alpha) {
        __stepper_alpha->setDirectionPin(PIN_ALPHA_DIR);   //for alpha=26, for beta = 14
        __stepper_alpha->setEnablePin(PIN_STEPPER_ENABLE);                            
        __stepper_alpha->setAutoEnable(false);

        __stepper_alpha->setSpeedInUs(1000);  // the parameter is us/step !!!
        __stepper_alpha->setAcceleration(100);
        // int res =  __stepper_alpha->moveTo(-1000, false);
        // Logger::Print("moveTo() returns", res);
        Logger::Info("TeethWarehouse_Board::Init()");
        Logger::Print("stepper alpha is OK.", 0);
    }else{
        Logger::Error("TeethWarehouse_Board::Init() ");
        Logger::Halt("failed FastAccelStepper.");
    }

    if (__stepper_beta) {
        __stepper_beta->setDirectionPin(PIN_BETA_DIR);   //for alpha=26, for beta = 14
        __stepper_beta->setEnablePin(PIN_STEPPER_ENABLE);                          
        __stepper_beta->setAutoEnable(false);

        __stepper_beta->setSpeedInUs(1000);  // the parameter is us/step !!!
        __stepper_beta->setAcceleration(100);
        Logger::Info("TeethWarehouse_Board::Init()");
        Logger::Print("stepper alpha is OK.", 0);
    }else{
        Logger::Error("TeethWarehouse_Board::Init() ");
        Logger::Halt("failed FastAccelStepper.");
    }
}

Servo* TeethWarehouse_Board::GetServo_onVertical(){
    return &__all_servos[SERVO_EEF_VERTICAL];
}

void TeethWarehouse_Board::EnableVacuumPump(bool enable_it){

    Logger::Print("TeethWarehouse_Board::EnableVacuumPump()  enable_it", enable_it );
    digitalWrite(PIN_VACUUME_PUMP, !enable_it);
}

void TeethWarehouse_Board::Test_PositionTriggers(int loops){
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

void TeethWarehouse_Board::Test_SingleStepper(int index, int loops){
    FastAccelStepper* stepper;
    if (index==0)  stepper = __stepper_alpha;
    if (index==1)  stepper = __stepper_beta;
    stepper->enableOutputs();
    for (int i=0; i<loops; i++){
        Logger::Print("Test stepper loop=", i);
        if (stepper) {
            // stepper->setDirectionPin(dirPinStepper);
            // stepper->setEnablePin(enablePinStepper);
            // stepper->setAutoEnable(true);

            // If auto enable/disable need delays, just add (one or both):
            // stepper->setDelayToEnable(50);
            // stepper->setDelayToDisable(1000);

            // __stepper_alpha->setSpeedInUs(1000);  // the parameter is us/step !!!
            // __stepper_alpha->setAcceleration(100);
            stepper->moveTo(1000, false);
            while (stepper->isRunning()){
                Logger::Print("Current position", stepper->getCurrentPosition());
                delay(300);
            }
            stepper->moveTo(-1000, false);
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

void TeethWarehouse_Board::Test_DualStepper(int loops){
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

void TeethWarehouse_Board::Test_Hx711(int loops){

    __hx711.set_scale(2280.f);          // this value is obtained by calibrating the scale with known weights; see the README for details
    __hx711.tare();				        // reset the scale to 0

    Serial.print("one reading:\t");
    Serial.print(__hx711.get_units(), 1);
    Serial.print("\t| average:\t");
    Serial.println(__hx711.get_units(10), 1);

    __hx711.power_down();			        // put the ADC in sleep mode
    delay(5000);
    __hx711.power_up();
}

void TeethWarehouse_Board::Test_Servo_AirPen(int loops){
    int pos = 0;      // position in degrees
    for (int loop=0; loop<loops; loop++){
        Logger::Print("SERVO_EEF_VERTICAL  count", loop);
        // Serial.println("Air pen loop");
        // for (pos = 0; pos <= 270; pos += 1) { // sweep from 0 degrees to 180 degrees
        //     // in steps of 1 degree
        //     __servo_air_pen.write(pos);
        //     delay(10);             // waits 20ms for the servo to reach the position
            
        // }
        // __servo_air_pen.write(0);
        __all_servos[SERVO_EEF_VERTICAL].write(0);
        delay(3000);
        // __servo_air_pen.write(270);
        __all_servos[SERVO_EEF_VERTICAL].write(270);
        delay(3000);
    }
}

void TeethWarehouse_Board::Test_Servo_VacuumSwitch(int loops){
    for (int loop=0; loop<loops; loop++){
        Serial.print("Air switch loop = ");
        Serial.print(loop);
        Serial.print("  Enable   ");
        EnableVacuumSwitch(true);
        delay(3000);
        Serial.print("  Disable   ");
        EnableVacuumSwitch(false);
        delay(3000);
        Serial.println("");
    }
}

void TeethWarehouse_Board::EnableVacuumSwitch(bool enable_it){
    int angle = 0;
    if (enable_it) angle = 270;
    if (enable_it) angle = 5;
    // __servo_air_switch.write(angle);
    __all_servos[SERVO_VACUUM_SWITCH].write(angle);
}

void TeethWarehouse_Board::Test_VacuumPump(int loops){
    for (int loop=0; loop<loops; loop++){
        Serial.print (loop);
        // turn on
        digitalWrite(PIN_VACUUME_PUMP, LOW);
        Serial.print("\t\tON    ");
        delay(5000);
        // turn off
        digitalWrite(PIN_VACUUME_PUMP, HIGH);
        Serial.print("OFF    ");
        delay(5000);

        Serial.println("");
    }
}

void TeethWarehouse_Board::Test_VL6180x(){
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
