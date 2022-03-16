#include "track_sensor_dual_9960.h"

TrackSensor_Dual9960::TrackSensor_Dual9960(uint8_t left_sensor_pin_sda, uint8_t left_sensor_pin_sclk, uint8_t right_sensor_pin_sda, uint8_t right_sensor_pin_sclk){
    this->__apds_left = new Adafruit_APDS9960();
    TwoWire* theWire = new TwoWire(0);
    theWire->begin(left_sensor_pin_sda, left_sensor_pin_sclk, 400000);
    this->__apds_left->begin(10, APDS9960_AGAIN_4X, APDS9960_ADDRESS, theWire);
    this->__apds_left->enableColor(true);

    this->__apds_right = new Adafruit_APDS9960();
    theWire = new TwoWire(1);
    theWire->begin(right_sensor_pin_sda, right_sensor_pin_sclk, 400000);
    this->__apds_right->begin(10, APDS9960_AGAIN_4X, APDS9960_ADDRESS, theWire);
    this->__apds_right->enableColor(true);

}
// when move from left to right, erro is from nagtive to positive.
float TrackSensor_Dual9960::ReadError_LeftRight(){
    uint16_t r,g,b,c;
    float track_error;
    Adafruit_APDS9960* sensor = this->__apds_right;
    if (this->__folking == FOLKING::FOLLOWING_LEFT){
        sensor = this->__apds_left;
    }
    sensor->getColorData(&r, &g, &b, &c);

    track_error = -32768 + c;
    if (this->__folking == FOLKING::FOLLOWING_LEFT){
        track_error = 32768 - c;
    }

    return track_error;
}

// when move forward, error is from negtive to positive.
float TrackSensor_Dual9960::ReadError_FrontRear(){
    uint16_t rr,rg,rb,rc;
    uint16_t lr,lg,lb,lc;
    this->__apds_left->getColorData(&lr, &lg, &lb, &lc);
    this->__apds_right->getColorData(&rr, &rg, &rb, &rc);

    float track_error;
    track_error = rr - rg;

    return track_error;
}

void TrackSensor_Dual9960::ClearFlag_Slowdown(){
    this->__flag_slow_down++;
}
void TrackSensor_Dual9960::ClearFlag_SpeedUp(){
    this->__flag_spped_up++;
}