#include "mover_dual_wheel.h"
#include <stdint.h>

MoverDualWheel::MoverDualWheel(){

}

void MoverDualWheel::SetForwdingSpeed(uint8_t forwarding_speed_in_percent, float angle){
    // Serial.print("[Debug] MoverDualWheel::SetForwdingSpeed()  forwarding_speed_in_percent= ");
    // Serial.println(forwarding_speed_in_percent);
    
    this->__leftDriver->SetVelocity_in_percent(forwarding_speed_in_percent);
    this->__rightDriver->SetVelocity_in_percent(forwarding_speed_in_percent);
}
