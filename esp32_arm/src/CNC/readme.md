# About CNCBase
* I know my child can do somthing, so these are my virual methods.
  * How many axis I have.
  * Translate gcode to motor movement
  * IK and FK
  * Homing process

# I don't know how to do it, Link me some helpers, like Board, Machine, EEF.
I don't know hardware gpio, eef
* Board
  * Borad can read sensors.
    * Link one or more homer (not sensor or gpio).
  * Board can do some action.
    * Move a stepper motor
      * Link one or more stepper driver to me.
      * The driver will set gpio HIGH, or Low.
* Machine config
  * Gearbox of motors.
  * Maxmum speed of movement , or homing movement.
* Link me one or more EEF (like Servo driver or H-Bridge driver) driver.
  * eef will let the board to drive hardware.


 
