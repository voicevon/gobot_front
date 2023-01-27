from von.remote_var_mqtt import RemoteVar_mqtt
from wcs_robots.gcode_sender import GcodeSender, gcode_senders
from wcs_robots.twh_robot_packer import TwhRobot_PackBox

class TwhRobot_Row():

    def __init__(self, robot_id:str, row_id:int) -> None:
        self.id = row_id
        self.robot_id = robot_id
        self.state_topic = "twh/" + robot_id + '/r' + str(row_id) + "/state"  #'twh/221109/r0/state'
        self.state = RemoteVar_mqtt(self.state_topic, 'idle')

        gcode_topic = "twh/" + robot_id + '/r' + str(row_id) + "/gcode"  #'twh/221109/r0/state'
        self.gcode_sender = GcodeSender(gcode_topic)
        gcode_senders.append(self.gcode_sender)
        self.target_col = -1
        self.target_layer = -1
        self.connected_packing_box = None

    def move_to(self, col:int, layer:int, connected_pack_box: TwhRobot_PackBox) -> None:
        self.target_col = col
        self.target_layer = layer
        self.connected_packing_box = connected_pack_box

        print("TwhRobot_Row::move_to()", 'row, col, layer = ' ,self.id, col, layer )
        self.state.set('moving')    # set to 'moving' when gcode-G1 is sent. ??
        
        mcode ='M42P99S1'  # turn off all green leds
        self.gcode_sender.append_gmcode_to_robot(mcode)

        gcode = 'G1X' + str(col)
        self.gcode_sender.append_gmcode_to_robot(gcode)

        mcode = 'M408' + self.state_topic
        self.gcode_sender.append_gmcode_to_robot(mcode)

    def show_layer_led(self):
        mcode = 'M42P' + str(self.target_layer) + 'S1'
        self.gcode_sender.append_gmcode_to_robot(mcode)

