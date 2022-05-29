# What I know
* Translate gcode to motor movement
* IK and FK
* Homing process

# What I don't know
* Hardware gpio
    * Link one or more stepper driver to me.
        * The driver will set gpio HIGH, or Low.
    * Link one or more homer (not sensor or gpio).
    * Link me one or more EEF (like Servo driver or H-Bridge driver) driver.
 


 /**
 * RobotBase has NO axis! 
 *      Reasons:
 *          1. Doesn't know how may axis. 
 *      ?? Or has at least one axis? 
 *      ?? Saying has no actuator, driver, sensor ?
 * RobotBase has NO ActuatorController!
 *          The ActuatorController might be inside of ActuatorDriver.
 *                                 might be no ActuatorCotroller in the whole system.
*/