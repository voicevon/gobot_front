#pragma once

#include "WString.h"
#include "Mqtt/mqtt_publisher_base.h"

// class Button_Gpio: public RemoteVar_Masterbase{
class Button_Gpio:  public MqttPublisherBase {
    public:
        Button_Gpio(int gpio_id);
        // void Init_Remotable(const char* mqtt_topic);
        bool onChanged(bool is_pressed); // {__callback = callback;};  
        void SpinOnce();
        bool IsPressed();
        bool IsToPressed(){return true;};  //TODO:  onPressed, onReleased.

    private:
        int __pin_number;
        bool __last_state_is_pressed = false;
        void* __callback = nullptr;   // TODO:  callback()  will return if continue to remote sync.

        // const char* _mqtt_topic = nullptr;
        String _PAYLOAD_STRING_ON = "ON";   //TODO:   be static.
        String _PAYLOAD_STRING_OFF = "OFF";

};