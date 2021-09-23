#include "all_devices.h"
#ifdef I_AM_CABLEBOT_CORNER

#include "MyLibs/MyFunctions.hpp"
#include "cablebot_corner.h"
#include "Robot/Gcode.h" 

#define PIN_ENCODER_A 19
#define PIN_ENCODER_B 18

CableBotCorner robot = CableBotCorner(AXIS_NAME);
IrEncoder irEncoder = IrEncoder(PIN_ENCODER_A, PIN_ENCODER_B, 16);
IrEncoderHelper irEncoderHelper = IrEncoderHelper();
void doB(){irEncoder.handleB();}




void setup(){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  CableBot-Corner.  Keep smiling :)");
    irEncoder.pullup= Pullup::USE_EXTERN;
    irEncoder.init();
    irEncoder.enableInterrupts(doB);
    irEncoderHelper.LinkIrEncoder(&irEncoder);
    // irEncoderHelper.InitFormula_LinearEquation(1.0f, 0.0f);
    irEncoderHelper.InitFormula_LinearEquation(180.0f * 22/ 7, 0.0f);

    robot.Init(&irEncoderHelper);
    Serial.println ("\n\nSetup is done. ------------------------------------ ");
}

void loop(){
    
    float a =irEncoder.getAngle()  / 6.28318530718 ;
    // long a = irEncoder.Get_countet();
    // float a = irEncoderHelper.GetMeanValue();
    Serial.println(a);
    delay(500);
    // Sensor s= Encoder();
    // Encoder e = Encoder();
    
    // robot.SpinOnce();

    //for testing.
    // Gcode gcode = Gcode("G1 X100");
    // robot.RunGcode(&gcode);

}

#endif