#include "track_sensor_dual_9960.h"

TrackSensor_Dual9960::TrackSensor_Dual9960(TrackSensor_Dual9960_Config* config){
    this->__config = config;
    this->left_sensor = new SmartLightSensor(0, config->pin_left_sensor_sda ,config->pin_left_sensor_sclk);
    this->right_sensor = new SmartLightSensor(1, config->pin_right_sensor_sda, config->pin_right_sensor_sclk);
    pinMode(config->pin_led, OUTPUT);
    digitalWrite(config->pin_led, LOW);
    
    this->IsFollowingLeft = true;
    this->pixels = new Adafruit_NeoPixel(config->LedWs2812B_counts, config->pin_led, NEO_GRB + NEO_KHZ800);
    this->pixels->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void TrackSensor_Dual9960::TurnOnLed(bool turn_on){
    if (turn_on)
        // digitalWrite(this->__config->pin_led, HIGH);
        // The first NeoPixel in a strand is #0, second is 1, all the way up
        // to the count of pixels minus one.
        for(int i=0; i< this->__config->LedWs2812B_counts; i++) { // For each pixel...
            // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
            // Here we're using a moderately bright green color:
            this->pixels->setPixelColor(i, pixels->Color(255, 255, 255));
            this->pixels->show();   // Send the updated pixel colors to the hardware.
        }
    else
        // digitalWrite(this->__config->pin_led, LOW);
          this->pixels->clear(); // Set all pixel colors to 'off'
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
