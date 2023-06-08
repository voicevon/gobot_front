#pragma once

#include "Robot/board/board_base.h"
#include "Robot/board/display/mono_led_gpio.h"
#include "Robot/Sensor/position_binary_trigger/position_trigger.h"
#include "CNC/Actuator/dc_motor/cnc_actuator_dc_motor.h"
#include "CNC/Actuator/dc_motor/h_bridge/h_bridge.h"
#include <SimpleFOC.h>
#include <SoftwareSerial.h>



class WaterDropper_Board: public BoardBase{
    public:
        void Init(const char* app_welcome_statement) override;
        void LinkEncoder(Encoder* encoder){__encoder=encoder;};
        Mono_Led_GPIO* GetStateLed(){return &__state_led;};
        PositionTrigger* Get_WebConfigButton(){return &__webconfig_button;};
        PositionTrigger* Get_ActionButton(){return &__action_button;};
        Encoder* Get_Encoder(){return __encoder;};
        
    private:
        void __InitHardware();
        Mono_Led_GPIO __state_led;
        PositionTrigger __webconfig_button;
        PositionTrigger __action_button;
        CncActuatorDcMotor __motor;
        Encoder* __encoder;
        H_Bridge __h_bridge;


        // SoftwareSerial mast_serial;
        // EspSoftwareSerial::UART master_serial;
        // EspSoftwareSerial::UART slave_serial;


};

extern WaterDropper_Board water_dropper_board;