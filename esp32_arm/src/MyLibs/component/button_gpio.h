#pragma once

class GpioButton{
    public:
        GpioButton(int gpio_id);
        void SpinOnce();
        bool IsPressed();
        bool IsToPressed(){return __is_changing_to_pressed;};  // should be callback?
        
    private:
        int __pin_number;
        bool __last_state_is_pressed = false;
        bool __is_changing_to_pressed = false;


};