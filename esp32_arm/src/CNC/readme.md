# About CNC
* CNC understand gcode, can translate gcode to movement, So there is a mover!
## Linked components, circle == 1
* Mover
* Actuator

## Linked components, circle == 2
* Homer and EEF are the outside components of CNC, saying they are linked components


# About CNCBase
* I know my child can do somthing, so these are my virual methods.
  * How many axis I have.
  
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


 
