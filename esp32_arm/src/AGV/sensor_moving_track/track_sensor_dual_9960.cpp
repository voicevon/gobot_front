#include "track_sensor_dual_9960.h"

// TrackSensor_Dual9960::TrackSensor_Dual9960(TrackSensor_Dual9960_Config* config){
//     this->__config = config;
//     this->left_sensor = new Smarter9960(0, config->pin_left_sensor_sda ,config->pin_left_sensor_sclk);
//     this->right_sensor = new Smarter9960(1, config->pin_right_sensor_sda, config->pin_right_sensor_sclk);

//     this->_Init();
// }

// TrackSensor_Dual9960::TrackSensor_Dual9960(TwoWire* i2c_bus_left, TwoWire* i2c_bus_right){
//     this->left_sensor = new Smarter9960(i2c_bus_left);
//     this->right_sensor = new Smarter9960(i2c_bus_right);
//     this->_Init(); 
// }
TrackSensor_Dual9960::TrackSensor_Dual9960(Adafruit_APDS9960* left_apds9960, Adafruit_APDS9960* right_apds9960){
    this->left_sensor = new Smarter9960(left_apds9960);
    this->right_sensor = new Smarter9960(right_apds9960);
}


void TrackSensor_Dual9960::_Init(){
    this->IsFollowingLeft = true;
}


bool TrackSensor_Dual9960::GetFlag_Slowdown(){
    if (this->__flag_slow_down == 1) 
        return true; 
    return false;
}
bool TrackSensor_Dual9960::GetFlag_Speedup(){
    if (this->__flag_spped_up == 1) 
        return true; 
    return false;
}


int16_t TrackSensor_Dual9960::ReadAlignmentError(){
    this->left_sensor->ReadSensor();
    this->right_sensor->ReadSensor();
    bool debug = false;
    if(debug){
        Serial.print("[Info] TrackSensor_Dual9960::ReadForwardingError()  left_sensor_light_percent= ");
        Serial.print(this->left_sensor->light_percent);
        Serial.print("   Right_sensor_light_percent= ");
        Serial.print(this->right_sensor->light_percent);
        Serial.print("   ");
    }
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
