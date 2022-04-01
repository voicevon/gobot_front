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

    // this->__current_sensor =  this->__apds_left;
    this->__current_sensor =  this->__apds_right;

}

void TrackSensor_Dual9960::ReadSensor(){
    Adafruit_APDS9960* sensor = this->__apds_right;
    if (this->IsFollowingLeft) 
        sensor = this->__apds_left;
    while(!sensor->colorDataReady()){
        delay(5);
    }

    sensor->getColorData(&this->color_r, &this->color_g, &this->color_b, &this->color_c);
    if (true){
        Serial.print(r);
        Serial.print("   ");
        Serial.print(g);
        Serial.print("   ");
        Serial.print(b);
        Serial.print("   ");
        Serial.print(c);
        Serial.print("             ");
        if (! this->IsFollowingLeft)
            Serial.println("");
    }
}

int16_t TrackSensor_Dual9960::ReadForwardingError(){
    //check slowdown, speedup, change following.
    // return x_error
    // uint16_t r,g,b,c;
    int16_t x_error;

    if (this->color_r > this->color_b + this->color_g){
        // got red mark
        if (this->__flag_slow_down == 0){
            this->__flag_slow_down = 1 ;
        }
    }
    if (this->color_b > this->color_r + this->color_g){
        // got blue mark
        if (this->__flag_spped_up == 0){
            this->__flag_spped_up  = 1;
        }
    }
    
    return x_error;
}

// when move from left to right, erro is from nagtive to positive.
// when move forward, error is from negtive to positive.
void TrackSensor_Dual9960::ReadParkingError(int16_t* x_error, int16_t* y_error){
    // return x_error, y_error
    uint16_t rr,rg,rb,rc;
    uint16_t lr,lg,lb,lc;
    this->__apds_left->getColorData(&lr, &lg, &lb, &lc);
    this->__apds_right->getColorData(&rr, &rg, &rb, &rc);

    float track_error;
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
