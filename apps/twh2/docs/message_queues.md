# AMQ: twh_deposite
- Group name: command_queues
  - Message Procuder: NodeRed (from UI)
  - Message Consumer: teeth_wh_robot.py
- Example
  - { "dev":"221109", "command":"prepare_deposit", "cell":{}};
  - { "dev":"221109", "command":"start_deposit", "cell":{ "row":123, "col":4 } }

# AMQ: twh_withdraw
- Group name: command_queues
  - Message Procuder: NodeRed (from UI)
  - Message Consumer: teeth_wh_robot.py
- Example
  - { "dev":"221109", "command": "withdraw", "cell": { "row": 1, "col": 1 } };
  - { "dev":"221109", "command": "end_withdraw"}

# AMQ: twh_221109_gcode
- Message Procuder: NodeRed (from command_queues)
- Message Consumer: teeth_wh_robot.py
- Example
  - "G1X123Y45"
  - "G4S3"
  - "M408"
 
# MQTT: and payload examples: 
- topic twh/221109/gcode_feed
  - "G1X123Y4", "G4S5", "M408"
- twh/221109/gcode_feed/fb
  - "G1X123Y4", "G4S5", "M408"
- twh/221109/ir_state
  - "empty", "blocked"

