#include "Axis_Stepper.h"

AxisStepper::AxisStepper(uint8_t _id){
    id = _id;
}
void AxisStepper::LinkMotor(Stepper* _stepper){
    motor = _stepper;
}

// void AxisStepper::__Init_motor(){
//   motor = new DCMotor();
//   motor->linkDriver(driver);
//   motor->linkSensor(encoder);
//   motor->controller = MotionControlType::angle;
//   motor->P_angle.P = 1;
//   motor->P_angle.I= 1;
//   motor->P_angle.D = 1;

//   // encoder->enableInterrupts(MoveAsync, MoveAsync);

// }
// void AxisStepper::Init(DCDriverHBridge* _driver, Encoder* _encoder, uint8_t pin_home){
//   driver = _driver;
//   encoder = _encoder;
//   __Init_motor();

//   __pinHomeSensor = pin_home;

// }

// void AxisStepper::Init(uint8_t motor_pinA, uint8_t motor_pinB, uint8_t encoder_pinA, uint8_t encoder_pinB, uint8_t pin_home ){
//   driver= new DCDriverHBridge(motor_pinA, motor_pinB);
//   encoder = new Encoder(encoder_pinA,encoder_pinB,200); 

//   __Init_motor();
//   __pinHomeSensor = pin_home;
// }
