#include "all_devices.h"
#ifdef I_AM_CABLEBOT_CORNER

#include "MyLibs/MyFunctions.hpp"
#include "cablebot_corner.h"
#include "Robot/Gcode.h"


CableBotCorner robot = CableBotCorner(AXIS_NAME);
Encoder encoder = Encoder(PIN_ENCODER_A_2130, PIN_ENCODER_B_2130, 200);
void doA(){encoder.handleA();}
void doB(){encoder.handleB();}

void setup(){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  CableBot-Corner.  Keep smiling :)");
    encoder.enableInterrupts(doA,doB);
    robot.Init(&encoder);
    Serial.println ("\n\nSetup is done. ------------------------------------ ");
}

void loop(){
    robot.SpinOnce();

    //for testing.
    Gcode gcode = Gcode("G1 X100");
    // robot.RunGcode(&gcode);

}

#endif