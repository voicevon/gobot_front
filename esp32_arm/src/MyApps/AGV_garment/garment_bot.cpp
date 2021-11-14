#include "all_devices.h"
// #ifdef I_AM_AGV_GARMENT


#include "garment_bot.h"

GarmentBot::GarmentBot(){
   PIDController* wheel_pid = new PIDController(1.0f, 1.0f, 0.0f ,80.0f, 100.0f);
   this->agv.Init(wheel_pid);
}


void GarmentBot::Init_Linkage(IrEncoderHelper* sensorHelper){

}







// #endif