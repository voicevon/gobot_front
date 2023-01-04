#include "twh4_ship_out_robot.h"
#include "Robot/mcode_runner/mcode_os.h"
#include "CNC/Actuator/dc_motor/actuator_encoder_calculator.h"
#include "IoT/wifi_mqtt_client.h"


void Twh4_ShipOut_Robot::MySpinOnce(){
}

void Twh4_ShipOut_Robot::Init(Twh4_ShipOut_Board* board){
    Logger::Debug("Twh4_ShipOut_Robot::Init()");
    // McodeOS::Instance().LinkRunner(&__m408_runner);
}
