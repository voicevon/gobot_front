#pragma once

#include "WString.h"
// #include "MyLibs/Mqtt/mqtt_publisher_base.h"
#include "MyLibs/mqtt/mqtt_publisher_base.h"


class Button_Gpio:  public MqttPublisherBase {
    public:
        Button_Gpio(int gpio_pin_number);
        bool onChanged(bool is_pressed); // {__callback = callback;};  
        void SpinOnce();
        bool IsPressed();
        bool IsToPressed(){return true;};  //TODO:  onPressed, onReleased.
        void setting_mqtt_publish_on_event(bool publish_it) {__setting_mqtt_publish_on_event = publish_it;};
        void setting_mqtt_publish_off_event(bool publish_it) {__setting_mqtt_publish_off_event = publish_it;};
        void setting_mqtt_publish_interval(int interval_ms) {__setting_mqtt_publish_interval_ms=interval_ms;};

    private:
        int __pin_number;
        bool __last_state_is_pressed = false;
        void* __callback = nullptr;   // TODO:  callback()  will return if continue to remote sync.
        bool __setting_mqtt_publish_on_event = true;
        bool __setting_mqtt_publish_off_event = false;
        uint32_t __setting_mqtt_publish_interval_ms = 5000;
        unsigned long __last_publish_timestamp = 0;

        void __mqtt_publish_state(bool is_on);


        // const char* _mqtt_topic = nullptr;
        String _PAYLOAD_STRING_ON = "ON";   //TODO:   be static.
        String _PAYLOAD_STRING_OFF = "OFF";


};