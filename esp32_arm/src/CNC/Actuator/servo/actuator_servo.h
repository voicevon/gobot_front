#pragma once

#include "../actuator_base.h"
#include <ESP32Servo.h>


class ActuatorServo: public ActuatorBase{
    public:
        float Read();
        void Write(float angle);
    protected:

    private:
        Servo __servo;
};