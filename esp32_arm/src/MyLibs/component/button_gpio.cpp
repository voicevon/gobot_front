#include "button_gpio.h"
#include "Arduino.h"
#include "MyLibs/basic/logger.h"
#include "Mqtt/wifi_mqtt_client.h"

Button_Gpio::Button_Gpio(int gpio_pin_number){
    __pin_number = gpio_pin_number;
    pinMode(__pin_number, INPUT_PULLUP);
}

// check button state, might publish mqtt message when its state is changed.
void Button_Gpio::SpinOnce(){
    bool is_pressed = ! digitalRead(__pin_number);
    // Logger::Debug("Button_Gpio::SpinOnce()");
    // Logger::Print("__pin_number", __pin_number);
    // Logger::Print("is_pressed", is_pressed);
    if (is_pressed != __last_state_is_pressed){
        // Logger::Print("Button_Gpio::SpinOnce()  is_pressed", is_pressed);
        // if (__callback != nullptr){
        if (true){
            bool go_on_remote = true;  // TODO:   *__callback();
            // *__callback();
            if (go_on_remote){
                if(_mqtt_publish_topic != nullptr){
                    // Logger::Print("Button_Gpio::SpinOnce()  is_pressed", is_pressed);
                    if(is_pressed) {
                        g_mqttClient.publish(_mqtt_publish_topic, 2, true,_PAYLOAD_STRING_ON.c_str());
                    }else{
                        g_mqttClient.publish(_mqtt_publish_topic, 2, true,_PAYLOAD_STRING_OFF.c_str());
                    }
                }
            }
        }
    } 
    __last_state_is_pressed = is_pressed;
}


bool Button_Gpio::IsPressed(){
    SpinOnce();
    return __last_state_is_pressed;
}