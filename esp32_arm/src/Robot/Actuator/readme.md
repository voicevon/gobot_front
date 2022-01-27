# What is an actuator?
* Is a movable thing.
## From machenical point of view:
 * Must include at least one motor, might be stepper, dc-motor, BLDC-motor
 * Might include a gearbox, axis couppler, etc.
## From electronic point of view:
 * A motor driver, might be 
   * Stepper driver: like A4988,Drv8825
   * A Dc-Motor driver: like L298N, YX45??, 
   * A BLDC driver: like O-Driver,
## From firmware point of view:
* Interface: can be GPIO, SPI/I2C/UART bus
 
## Involved
Some features are very related to actuator, but we don't consider them as a part of ActuatorBase.
The instance is possible to use multi hirachy from other parent class.
* Openloop vs Closeloop
 * Instance of closed loop: servo
 * Instance of open loop: stepper drived thing. 
* Sensors:
 * Position sensor, 
 * Homing sensor,
# Class hirachy
```
    ActuatorBase
       |---- Actuator_DcMotor
       |---- Actuator_Stepper
       |---- Actuator_Bldc
```

# Detail explain
* An actuator is a phasical movement component, 
 * Knows: Inverse kinematic values, like: position, axis name
 * Unknows: Forword kinematic position, axis name,
