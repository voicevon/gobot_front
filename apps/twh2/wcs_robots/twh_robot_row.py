from wcs_robots.gcode_sender import GcodeSender
from von.mqtt_auto_sync_var import MqttAutoSyncVar


class TwhRobot_Row():
    def __init__(self, robot_id, row_id:int) -> None:
        self.id = row_id
        state_topic = "twh/" + str(robot_id) + '/r' + str(row_id) + "/state"  #'twh/221109/r0/state'
        self.state = MqttAutoSyncVar(state_topic, 'idle')
        self.is_moving = False
        self.gcode_sender = GcodeSender()

    def spin_once(self):
        pass
   


    def move_to(self, withdraw_item) -> None:
        GcodeSender.send_code('xxx')

    

