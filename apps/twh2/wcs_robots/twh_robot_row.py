# from wcs_robots.gcode_sender import GcodeSender
from von.mqtt_auto_sync_var import MqttAutoSyncVar
from von.amq_agent import g_amq


class TwhRobot_Row():
    def __init__(self, robot_id, row_id:int) -> None:
        self.id = row_id
        self.robot_id = robot_id
        state_topic = "twh/" + str(robot_id) + '/r' + str(row_id) + "/state"  #'twh/221109/r0/state'
        self.state = MqttAutoSyncVar(state_topic, 'idle')

    def move_to(self, col:int, layer:int) -> None:
        print("TwhRobot_Row::move_to()", 'row,layer = ' ,self.id, col, layer )
        topic = 'twh/' + str(self.robot_id) + '/r'+str(self.id) + "/gcode"
        payload = 'G1X' + str(col)
        g_amq.Publish(topic, payload)
        payload ='M42P' + str(layer) + 'S1'
        g_amq.Publish(topic, payload)
        payload = 'M408'
        g_amq.Publish(topic, payload)


    

