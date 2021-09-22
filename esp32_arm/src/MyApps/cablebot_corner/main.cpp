#include "all_devices.h"
#ifdef I_AM_CABLEBOT_CORNER

#include "MyLibs/MyFunctions.hpp"
#include "cablebot_corner.h"
#include "Robot/Gcode.h"

#define PIN_ENCODER_A 19
#define PIN_ENCODER_B 18

CableBotCorner robot = CableBotCorner(AXIS_NAME);
// Encoder encoder = Encoder(PIN_ENCODER_A, PIN_ENCODER_B, 8);
IrEncoder encoder = IrEncoder(PIN_ENCODER_A, PIN_ENCODER_B, 8);
// void doA(){encoder.handleA();}
void doB(){encoder.handleB();}



void setup(){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  CableBot-Corner.  Keep smiling :)");
    encoder.pullup= Pullup::USE_EXTERN;
    // encoder.quadrature = Quadrature::OFF;
    encoder.init();
    // encoder.enableInterrupts(doA,doB);
    encoder.enableInterrupts(doB);
    // robot.Init(&encoder);
    Serial.println ("\n\nSetup is done. ------------------------------------ ");
}

void loop(){
    
    float a =encoder.getAngle() * 360.0f;
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