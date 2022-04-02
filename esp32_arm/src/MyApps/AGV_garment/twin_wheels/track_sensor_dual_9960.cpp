#include "track_sensor_dual_9960.h"

TrackSensor_Dual9960::TrackSensor_Dual9960(uint8_t left_sensor_pin_sda, uint8_t left_sensor_pin_sclk, uint8_t right_sensor_pin_sda, uint8_t right_sensor_pin_sclk){
    this->left_sensor = new SmartLightSensor(0, left_sensor_pin_sda,left_sensor_pin_sclk);
    this->right_sensor = new SmartLightSensor(1, right_sensor_pin_sda, right_sensor_pin_sclk);
    this->IsFollowingLeft = true;

}


int16_t TrackSensor_Dual9960::ReadForwardingError(){
    this->left_sensor->ReadSensor();
    this->right_sensor->ReadSensor();
    Serial.print(this->left_sensor->light_percent);
    Serial.print("   ");
    Serial.print(this->right_sensor->light_percent);
    Serial.print("   ");
    int common_light_percent = 0;
    int16_t x_error = 100.0f * (this->left_sensor->light_percent - this->right_sensor->light_percent);
    if(this->left_sensor->light_percent > 0.2 && this->right_sensor->light_percent > 0.2){
        common_light_percent = 100.0f * min(this->left_sensor->light_percent, this->right_sensor->light_percent);
        if (x_error > 0)
            x_error += common_light_percent;
        else
            x_error -= common_light_percent;
    }
    return x_error;
}

// when move from left to right, erro is from nagtive to positive.
// when move forward, error is from negtive to positive.
void TrackSensor_Dual9960::ReadParkingError(int16_t* x_error, int16_t* y_error){
    // return x_error, y_error
    uint16_t rr,rg,rb,rc;
    uint16_t lr,lg,lb,lc;


    *x_error = rr - rg;
    *y_error = rr - lr;

}

void TrackSensor_Dual9960::ClearFlag_Slowdown(){
    this->__flag_slow_down++;
}
void TrackSensor_Dual9960::ClearFlag_SpeedUp(){
    this->__flag_spped_up++;
}

void TrackSensor_Dual9960::SayHello(){
    Serial.println("[Hello]  TrackSensor_Dual9960::SayHello()     ");
}
