#include "all_devices.h"
#ifdef I_AM_AGV_GARMENT

#include "MyLibs/MyFunctions.hpp"
#include "garment_bot.h"
#include "Robot/Gcode.h" 

#define PIN_ENCODER_A 25
#define PIN_ENCODER_B 26

GarmentBot robot = GarmentBot();
IrEncoder irEncoder = IrEncoder(PIN_ENCODER_A, PIN_ENCODER_B, 40);
IrEncoderHelper irEncoderHelper = IrEncoderHelper();
void doB(){irEncoder.handleB();}



void setup(){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  CableBot-Corner.  Keep smiling :)");
    irEncoder.pullup= Pullup::USE_EXTERN;
    irEncoder.init();
    irEncoder.enableInterrupts(doB);
    irEncoderHelper.LinkSensor(&irEncoder);
    irEncoderHelper.InitFormula_LinearEquation(3.613f, 0.0f);
    
    robot.Init_Linkage(&irEncoderHelper);
    Serial.println ("\n\nSetup is done. ------------------------------------ ");
}

bool tested = false;
void loop(){
    // bool x = digitalRead(32);
    // Serial.print(x);
    // delay(1000);
    // robot.SpinOnce();

    if (!tested){
        // robot.test_home();
        // tested = true;
        // Gcode gcode = Gcode("M119");
        // robot.RunGcode(&gcode);
        // delay(100);
    }
}

#endif