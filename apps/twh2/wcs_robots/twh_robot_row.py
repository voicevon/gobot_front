from von.mqtt_auto_sync_var import MqttAutoSyncVar
from wcs_robots.gcode_sender import GcodeSender, gcode_senders

class TwhRobot_Row():

    def __init__(self, robot_id:str, row_id:int) -> None:
        self.id = row_id
        self.robot_id = robot_id
        state_topic = "twh/" + robot_id + '/r' + str(row_id) + "/state"  #'twh/221109/r0/state'
        self.state = MqttAutoSyncVar(state_topic, '{"message_id":1,"is_moving":false}')
        self.state.Copy_LocalToRemote()

        gcode_topic = "twh/" + robot_id + '/r' + str(row_id) + "/gcode"  #'twh/221109/r0/state'
        self.gcode_sender = GcodeSender(gcode_topic)
        gcode_senders.append(self.gcode_sender)

    def move_to(self, col:int, layer:int) -> None:
        print("TwhRobot_Row::move_to()", 'row,layer = ' ,self.id, col, layer )
        gcode = 'G1X' + str(col)
        self.gcode_sender.append_gmcode_to_robot(gcode)

        gcode ='M42P' + str(layer) + 'S1'
        self.gcode_sender.append_gmcode_to_robot(gcode)

        gcode = 'M408'
        self.gcode_sender.append_gmcode_to_robot(gcode)

    

