#include "track_sensor_smart_9960.h"

SmartLightSensor::SmartLightSensor(uint8_t i2c_bus_id, uint8_t pin_sda, uint8_t pin_sclk){
    TwoWire* i2c_bus = new TwoWire(i2c_bus_id);
    i2c_bus->begin(pin_sda, pin_sclk, 400000);
    this->sensor = new Adafruit_APDS9960();
    this->sensor->begin(10, APDS9960_AGAIN_4X, APDS9960_ADDRESS, i2c_bus);
    this->sensor->enableColor(true);

    this->c_min = 999;
    this->c_max = 0;
    this->r_min = this->g_min = this->b_min = this->c_min;
    this->r_max = this->g_max = this->b_max = this->c_max;

}




void SmartLightSensor::ReadSensor(){
    //Step1:  Read raw value from hardware sehnsor
    while(!this->sensor->colorDataReady()){
        delay(5);
    }

    sensor->getColorData(&this->color_r, &this->color_g, &this->color_b, &this->color_c);
    bool debug= true;
    if (debug){

        Serial.print(this->color_r);
        Serial.print("   ");
        Serial.print(this->color_g);
        Serial.print("   ");
        Serial.print(this->color_b);
        Serial.print("   ");
        Serial.print(this->color_c);
        Serial.print("   ");
        Serial.print(this->c_min);
        Serial.print("   ");
        Serial.print(this->c_max);
        Serial.print("   ");
        // if (! this->IsFollowingLeft)
        //     Serial.println("");
    }

    //Step2: Learn environment
    if (this->c_max < this->color_c)
        this->c_max ++;
    if(this->c_min > this->color_c)
        this->c_min--;

    //Step3: Calibrate result.
    this->light_percent = (this->color_c - this->c_min) / (this->c_max - this->c_min);

}