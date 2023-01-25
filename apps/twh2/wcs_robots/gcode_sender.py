# from von.amq_agent import g_amq
from von.mqtt_agent import g_mqtt
from von.mqtt_auto_sync_var import MqttAutoSyncVar
import queue

class GcodeSender():

    def __init__(self, mqtt_topic:str) -> None:
        self.mqtt_topic = mqtt_topic
        # self.sending_message_id =  MqttAutoSyncVar(mqtt_topic=mqtt_topic + "/fb", default_value=0, var_data_type=int)
        self.__sending_message =  MqttAutoSyncVar(mqtt_topic=mqtt_topic + "/fb", default_value=None, var_data_type=int)
        self.__sending_message.Copy_LocalToRemote()
        self.queue = queue.Queue()
        self.message_id = 0

    def append_gmcode_to_robot(self, gcode:str):
        payload = {}
        payload['id'] = self.message_id
        payload['gmc'] = gcode
        self.queue.put(payload)
        self.message_id += 1

    # def append_command_to_app(self,command:str):
    #     payload = {}
    #     payload['id'] = self.message_id
    #     payload['cmd'] = command
    #     self.queue.put(payload)
    #     self.message_id += 1

    def spin_once(self):
        # if self.sending_message_id.local_value != self.sending_message_id.remote_value:
        if self.__sending_message.local_value != self.__sending_message.remote_value:
            # TODO:  time out and retry
            return
        if self.queue.empty():
            return
        # queue is not empty, and got last feedback correctly.
        payload = self.queue.get()
        # self.sending_message_id = payload['id']
        self.__sending_message = payload['gmc']
        g_mqtt.publish(self.mqtt_topic, payload=self.__sending_message)
