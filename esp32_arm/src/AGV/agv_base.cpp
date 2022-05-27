#include "agv_base.h"
#include <HardwareSerial.h>

void AgvBase::_InitBase(){
    this->ToState(PARKED);
}

void AgvBase::SetFollowMainRoad(bool next_branch_is_on_left, bool follow_main_road){
    this->trackSensor->IsFollowingLeft = !(next_branch_is_on_left ^ follow_main_road);
}

void AgvBase::Forwarding(){
    int16_t x_error = this->trackSensor->ReadForwardingError();
    // pid controller to set common_speed, diff_speed

    // float left_speed = this->common_speed + x_error;
    // float right_speed = this->common_speed - x_error;
    this->__mover->SetForwdingSpeed(this->common_speed, x_error);

}

void AgvBase::SpinOnce(){
    bool test_track_sensor_only = false;
    bool test_led_on = false;
    while (test_track_sensor_only){
        this->trackSensor->IsFollowingLeft = !this->trackSensor->IsFollowingLeft;
        int16_t xx_error = this->trackSensor->ReadForwardingError();
        Serial.println (xx_error);
        this->trackSensor->TurnOnLed(test_led_on);
        test_led_on = !test_led_on;
        delay(10000);
    }

    // Obstacle detection
    bool found_obstacle= this->obstacleSensor->DetectObstacle();


    switch (this->_State) {
    case FAST_MOVING:
        if (found_obstacle){
            this->ToState(FAST_MOVING_PAUSED);
        // check if see the mark of slow-down.
        }else if (this->trackSensor->GetFlag_Slowdown()){
            this->ToState(SLOW_MOVING);
            this->trackSensor->ClearFlag_Slowdown();
        }else{
            //going on with fast_moving.
            this->Forwarding();
        }
        break;
    case FAST_MOVING_PAUSED:
        if (!found_obstacle)
            this->ToState(FAST_MOVING);
        break;
    case SLOW_MOVING:
        if (found_obstacle){
            this->ToState(SLOW_MOVING_PAUSED);
            Serial.println("qqqqqqqqqqqqqqqqqqqqqqqqqqqq");
        }else if(this->trackSensor->GetFlag_Speedup()){
                this->ToState(FAST_MOVING);
        }else{
            this->Forwarding();
        }
        break;
    case SLOW_MOVING_PAUSED:
        if (!found_obstacle){
            this->ToState(SLOW_MOVING);
        }
        else{
            Serial.println("ttttttttttttttttttttttttt");
        }
        break;
    case PARKING:
            // try to finish parking
            if(this->DoParking()){
                this->ToState(PARKED);
            }
            break;
    case PARKING_PAUSED:
            if (!found_obstacle)
                this->ToState(PARKING);
            break;
    case PARKED:
            break;

    default:
        Serial.print("[Warn] AgvBase::SpinOnce()   Unhandld state= ");
        Serial.println(this->_State);
        break;
    }
}

void AgvBase::ToState(AGV_STATE state){
    if (state == this->_State) return;
    if (this->_State == PARKED){
        // leaving parked state
        this->trackSensor->TurnOnLed(true);
    }
    switch(state){
    case FAST_MOVING:
        this->common_speed = this->__fast_velocity;
        this->__mover->SetForwdingSpeed(80, 0.0f);
        break;
    case SLOW_MOVING:
        this->common_speed = this->__slow_velocity;
        this->__mover->SetForwdingSpeed(20, 0.0f);
        break;
    case PARKING:
        this->common_speed = this->__parking_velocity;
        break;
    case FAST_MOVING_PAUSED:
        this->__mover->SetForwdingSpeed(0, 0.0f);
        // this->leftWheel_serial->write("T0");
        // this->rightWheel_serial->write("T0");
        break;
    case SLOW_MOVING_PAUSED:
        this->__mover->SetForwdingSpeed(0, 0.0f);
        // this->leftWheel_serial->write("T0");
        // this->rightWheel_serial->write("T0");
        break;
    case PARKED:
        // this->leftWheel_serial->write("T0");
        // this->rightWheel_serial->write("T0");
        // this->trackSensor->TurnOnLed(false);
        break;
        
    default:
        Serial.print("[Warn] AgvBase::ToState()  switch without case ");
        Serial.println(state);
        break;
    }
    this->_State = state;
    Serial.print("\n[Info] GarmentBot::ToState()  NewState= " );
    Serial.println(state);
}

// return true:   reach parked position
// return false:  still parking
bool AgvBase::DoParking(){
    int16_t x_error ;
    int16_t y_error ;
    this->trackSensor->ReadParkingError(&x_error, &y_error);

    this->common_speed =  this->__parking_velocity;
    if (x_error < 10 && y_error < 10)
        return true;
    return false;
}