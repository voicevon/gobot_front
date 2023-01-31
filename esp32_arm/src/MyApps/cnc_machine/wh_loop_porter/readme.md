# This unit will manager only one robot unit of row.

# Only power is connected to master

# All communication will use Wifi or BLE.
## Verion 1: Will use MQTT over Wifi
* All row units subscribe a same topic, and same feedback topic.
  * MQTT Topic:  "twh/221109/row/gcode"
  * Feedback topic: "twh/221109/row/gcode/fb"
  * Message payload:  "G1R0C15"  
  * Only the target row will send feedback
