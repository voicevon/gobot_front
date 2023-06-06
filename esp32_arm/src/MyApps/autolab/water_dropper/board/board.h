#pragma once
#include "Robot/board/board_base.h"
#include "Robot/board/display/mono_led_gpio.h"
#include "Robot/Sensor/position_binary_trigger/position_trigger.h"
#include "CNC/Actuator/dc_motor/cnc_actuator_dc_motor.h"
#include <SimpleFOC.h>
#include <SoftwareSerial.h>



class WaterDropper_Board: public BoardBase{
    public:
        void Init(const char* app_welcome_statement) override;
        void LinkEncoder(Encoder* encoder){__encoder=encoder;};
        Mono_Led_GPIO* GetStateLed(){return &__state_led;};
        PositionTrigger* Get_ConfigButton(){return &__config_button;};
        PositionTrigger* Get_ActionButton(){return &__action_button;};
        
        
    private:
        void __InitHardware();
        Mono_Led_GPIO __state_led;
        PositionTrigger __config_button;
        PositionTrigger __action_button;
        CncActuatorDcMotor __motor;
        Encoder* __encoder;


        // SoftwareSerial mast_serial;
        // EspSoftwareSerial::UART master_serial;
        // EspSoftwareSerial::UART slave_serial;


};