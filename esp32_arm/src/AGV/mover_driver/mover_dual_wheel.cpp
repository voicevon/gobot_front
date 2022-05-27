#include "mover_dual_wheel.h"
#include <stdint.h>

MoverDualWheel::MoverDualWheel(){

}

void MoverDualWheel::SetForwdingSpeed(uint8_t forwarding_speed_in_percent, float angle){
    this->__leftDriver->SetVelocity_in_percent(forwarding_speed_in_percent);
    this->__rightDriver->SetVelocity_in_percent(forwarding_speed_in_percent);
}
