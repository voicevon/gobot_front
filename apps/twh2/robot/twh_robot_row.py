from gcode_sender import GcodeSender
from von.mqtt_auto_sync_var import MqttAutoSyncVar


class TwhRobot_Row():
    def __init__(self, robot_id, row_id:int) -> None:
        self.row_id = row_id
        state_topic = "twh/" + str(robot_id) + '/' + str(row_id) + "/state"
        self.state = MqttAutoSyncVar(state_topic, 'idle')
        self.gcode_sender = GcodeSender()

    def move_to(self, col: int) -> None:
        # GcodeSender.row_robot_move_to_col(self.row_id, col)
        # self.state.local_value = 'moving'
        # self.state.Copy_LocalToRemote()
        print("TwhRobot_Row::move_to()", col)

    def get_state(self) -> str:
        return self.state.remote_value


    

