# Message type
## TwhMessage_UserRequest
- Routing
 - From: NodeRed
 - To: Python
- Queues:
 - single queue
- Content
 - is_withdraw : bool
 - locations: list[x,y]

## TwhMessage_gCode
- Routing
 - From: Python
 - To: Hardware robot
- Queues
 - For each hardware robot
- Content:
 - device_id: int
 - gcode: string


## TwhMessage_Sensor_IR
- Routing
 - From: Hardware robot
 - To:  Python
- Queues
 - No Queue ?
- Content:
 - is_blocked:  bool


