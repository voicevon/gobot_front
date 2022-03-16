#include "track_sensor_dual_9960.h"

TrackSensor_Dual9960::TrackSensor_Dual9960(uint8_t left_sensor_pin_sda, uint8_t left_sensor_pin_sclk, uint8_t right_sensor_pin_sda, uint8_t right_sensor_pin_sclk){
    // this->__position = position;
    // this->__i2c_bus_at_left = new TwoWire(0);
    // this->__i2c_bus_at_right = new TwoWire(1);

    // this->__i2c_bus_at_left->begin(left_sensor_pin_sda, left_sensor_pin_sclk, 400000);
    // this->__i2c_bus_at_right->begin(right_sensor_pin_sda, right_sensor_pin_sclk);


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
float TrackSensor_Dual9960::ReadError_FrontRear(){
    return 1.0f;
}