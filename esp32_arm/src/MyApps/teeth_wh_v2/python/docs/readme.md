# check statemahine
- http://www.plantuml.com/plantuml/png/

# user command
## deposit_prepare
- Arm move to 0 degree
- Outbox Move to outside of the machine

## deposit_cancel
- Outbox Move to inside of the machine

## deposit_start (target_cell, teeth_count) 
- Outbox move to inside of the mahine
- repeat 
 - pickup from outbox
 - drop to target cell(row,col)
 

## start withdraw (source_cell list)
- enable vacuum fan
- repeat for each souce_cell
 - repeat for each trying
    - eef move to top
    - move to source_cell (row, col)
    - eef move down
    - enable air_sucker
    - eef move up to edge
    - draw a cicle, to cut off some teeth, only one is needed.
    - eef move to top
    - IR check, is there 0 or 1 tooth is air sucked
 - move axis A to 0 degree
 - disable vacumm sucker
- Move outbox to outside of the machine.






