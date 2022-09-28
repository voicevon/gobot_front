# Robot Main components and their jobs

## gCodeDispacher, Now is robot do it.
* Read gcode from gcode_queue, 
* Send gcode to involved component, 
 * Send to Planner for G1,G2,G4,G28
 * Send mcode to EEF, homer, pid_controllers.   etc

## Planner
* Cut LineSegment into move_block.
* For each move_block, recalculate speed, accelleration.
* Append move_blocks to queue_move_block.

### arm solution 
* help planner to archive how to cut line_segment
* FK and IK  inter translation.

## Mover
* read from queue_move_block.
* Control actuator, position_sensor.
 