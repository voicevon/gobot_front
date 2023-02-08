#pragma once

#include "remote_base/remotable_master_base.h"
#include "WString.h"

class Button_Gpio: public RemoteVar_Masterbase{
    public:
        Button_Gpio(int gpio_id);
        void Init_Remotable( char* mqtt_topic);
        void SpinOnce();
        bool IsPressed();
        bool IsToPressed(){return true;};  // should be callback?
        
    private:
        int __pin_number;
        bool __last_state_is_pressed = false;
        // bool __is_changing_to_pressed = false;


};