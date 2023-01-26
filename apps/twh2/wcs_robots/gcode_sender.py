from von.mqtt_agent import g_mqtt
import queue


class VonMessage():
    def __init__(self, id:int, payload:str) -> None:
        self.id = id
        self.payload = payload

class GcodeSender():

    def __init__(self, mqtt_topic:str) -> None:
        self.tx_topic = mqtt_topic
        self.rx_topic = mqtt_topic + '/fb'
        self.tx = VonMessage(0, None)
        self.rx = VonMessage(0, None)
        self.next_message_id = 0
        g_mqtt.append_on_received_message_callback(self.__on_mqtt_received)
        g_mqtt.subscribe(self.rx_topic, qos=2)

        self.queue = queue.Queue()

    def __on_mqtt_received(self, mqtt_message_topic, mqtt_message_payload ):
        if mqtt_message_topic != self.rx_topic:
            return
        self.tx.payload = mqtt_message_payload

    def append_gmcode_to_robot(self, tx_payload:str):
        tx_message = VonMessage(self.next_message_id, tx_payload)
        self.next_message_id += 1
        self.queue.put(tx_message)


    # def append_command_to_app(self,command:str):
    #     payload = {}
    #     payload['id'] = self.message_id
    #     payload['cmd'] = command
    #     self.queue.put(payload)
    #     self.message_id += 1

    def spin_once(self):
        if self.tx.payload == self.rx.payload:
            # TODO:  time out and retry
            return
        if self.queue.empty():
            # print('gcode_sender  spin_once()', self.tx.payload, self.rx.payload)
            return
        # queue is not empty, and got last feedback correctly.
        tx_von_message = self.queue.get()
        self.tx.id = tx_von_message.id
        self.tx.payload = tx_von_message.payload
        g_mqtt.publish(self.tx_topic, payload=self.tx.payload)
        print("gcode sender....", self.tx_topic, self.tx.id, self.tx.payload)


gcode_senders = []

def gcode_senders_spin_once():
    for gcode_sender in gcode_senders:
        gcode_sender.spin_once()
