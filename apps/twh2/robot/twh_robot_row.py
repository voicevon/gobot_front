from gcode_sender import GcodeSender
from von.mqtt_auto_sync_var import MqttAutoSyncVar


class TwhRobot_Row():
    def __init__(self, robot_id, row_id:int) -> None:
        self.row_id = row_id
        state_topic = "twh/" + str(robot_id) + '/r' + str(row_id) + "/state"  #'twh/221109/r0/state'
        self.state = MqttAutoSyncVar(state_topic, 'idle')
        self.gcode_sender = GcodeSender()
        self.current_state = 'idle'

    def spin_once(self):
        match self.current_state:
            case 'idle':
                pass
            case 'moving':
                if self.state.remote_value == 'idle':
                    self.current_state = 'ready'
            case 'ready':
                pass


    def move_to(self, withdraw_item) -> None:
        if self.current_state != 'idle':
            print('TwhRobot_Row::move_to()  Waring. current state')
        self.current_state = 'moving'
        # GcodeSender.row_robot_move_to_col(self.row_id, col)
        # self.state.local_value = 'moving'
        # self.state.Copy_LocalToRemote()
        self.withdraw_item = withdraw_item
        print("TwhRobot_Row::move_to()", withdraw_item['col'])


    def get_withdraw_item(self):
        return self.withdraw_item
    
