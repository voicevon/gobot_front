@startuml
[*] -> parking
parking--> picking_cell: withdraw
picking_cell <- picking_cell: ir_check_empty
picking_cell --> droping_centerbox : ir_check_blocked
droping_centerbox --> picking_cell: withdraw
droping_centerbox ->be_outside : end_withdraw
be_outside --> parking


droping_centerbox: vacuum_suck(disable)
droping_centerbox: dwell(4 seconds)
droping_centerbox: air_suck(enable)
parking: vacuum_pump(disable)
parking: move_to(x200)
parking: move_to(z0)
parking: home_arm



picking_cell: move_to(x,y)
picking_cell: vacuum_suck(enable)
picking_cell: move_to(z=100)
picking_cell: dwell (2 seconds)
picking_cell: move_to(z=0)
picking_cell: mqtt_publish(ir_state)


parking--> feeding_centerbox: prepare_deposit
feeding_centerbox --> axis_x: start_deposit
feeding_centerbox --> feeding_centerbox : prepare_deposit
feeding_centerbox -> parking: end_deposit
axis_x -> picking_centerbox
picking_centerbox --> droping_cell: ir_check_blocked
picking_centerbox --> picking_centerbox: ir_check_empty <=3
picking_centerbox --> verify: ir_check_emtpy > 3
droping_cell -> picking_centerbox: .         .
verify -> axis_x: white
verify -> parking: black

feeding_centerbox: home_arm()
feeding_centerbox: move_centerbox_outside()
axis_x: move_to(x=200)
picking_centerbox: move_to(A=0)
picking_centerbox: vacuum_suck(enable)
picking_centerbox: move_to(z=100)
picking_centerbox: dwell(3 seconds)
picking_centerbox: move_to(z=0)
picking_centerbox: mqtt_publish(ir_state)
droping_cell: move_to(a=33)
droping_cell: vacuum_suck(diable)
droping_cell: dwell(2 seconds)
verify: home_arm()
verify: move_to(x=299)
verify: mqtt_publish(color or image)



@enduml