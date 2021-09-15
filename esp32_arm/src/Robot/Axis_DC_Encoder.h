#ifndef _AXIS_DC_ENCODER_H_
#define _AXIS_DC_ENCODER_H_


#include "SingleAxisBase.h"

#include "SimpleFOC/SimpleFOC.h"

class Axis_DC_Encoder: public SingleAxisBase{
    public:
        Axis_DC_Encoder();
        Axis_DC_Encoder(uint8_t _id);

        DCMotor* motor;
        Encoder* encoder;
        DCDriver* driver;
        void Init(DCDriverHBridge* driver, Encoder* encoder, uint8_t pin_home);
        void Init(uint8_t motor_pinA, uint8_t motor_pinB, uint8_t encoder_pinA, uint8_t encoder_pinB, uint8_t pin_home );

    private:
        void __Init_motor();

};

#endif