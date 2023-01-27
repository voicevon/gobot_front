from von.remote_var_mqtt import RemoteVar_mqtt
from wcs_robots.gcode_sender import GcodeSender, gcode_senders

class TwhRobot_Row():

    def __init__(self, robot_id:str, row_id:int) -> None:
        self.id = row_id
        self.robot_id = robot_id
        self.state_topic = "twh/" + robot_id + '/r' + str(row_id) + "/state"  #'twh/221109/r0/state'
        self.state = RemoteVar_mqtt(self.state_topic, 'idle')

        gcode_topic = "twh/" + robot_id + '/r' + str(row_id) + "/gcode"  #'twh/221109/r0/state'
        self.gcode_sender = GcodeSender(gcode_topic)
        gcode_senders.append(self.gcode_sender)

    def move_to(self, col:int, layer:int) -> None:
        print("TwhRobot_Row::move_to()", 'row, col, layer = ' ,self.id, col, layer )
        self.state.set('moving')    # set to 'moving' when gcode-G1 is sent. ??
        gcode = 'G1X' + str(col)
        self.gcode_sender.append_gmcode_to_robot(gcode)

        gcode ='M42P' + str(layer) + 'S1'
        self.gcode_sender.append_gmcode_to_robot(gcode)

        gcode = 'M408' + self.state_topic
        self.gcode_sender.append_gmcode_to_robot(gcode)

    

