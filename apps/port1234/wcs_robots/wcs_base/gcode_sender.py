from von.mqtt.mqtt_agent import g_mqtt
from von.logger import Logger
import queue


class VonMessage():
    def __init__(self, id:int, payload:str) -> None:
        self.id = id
        self.payload = payload

class GcodeSender():
    '''
    TODO:  feed back via message_id.  Will effect Esp32
    '''
    def __init__(self, mqtt_topic:str) -> None:
        self.__tx_topic = mqtt_topic
        self.__rx_topic = mqtt_topic + '/fb'
        self.__waitting_msg = VonMessage(0, None)
        self.__rx = VonMessage(0, None)
        self.__next_message_id = 0
        g_mqtt.append_on_received_message_callback(self.__on_mqtt_received)
        g_mqtt.subscribe(self.__rx_topic, qos=2)

        self.__queue = queue.Queue()

    def __on_mqtt_received(self, mqtt_message_topic, mqtt_message_payload ):
        if mqtt_message_topic != self.__rx_topic:
            return
        # Logger.Info('GcodeSender.__on_mqtt_received()   Got feedback')
        # self.__rx.id = 12345;  #TODO:  archieve this.
        self.__rx.payload = mqtt_message_payload

        # For the first time, this function will receive the retained message, not the sent message.
        if self.__waitting_msg.id == 0:
            self.__waitting_msg.payload = mqtt_message_payload

    def append_gmcode_to_queue(self, gcode:str):
        # Logger.Debug("GcodeSender::append_gmcode_to_queue()     ")
        # Logger.Print('gcode', gcode)
        tx_message_in_queue = VonMessage(self.__next_message_id, gcode)
        self.__next_message_id += 1
        self.__queue.put(tx_message_in_queue)
        # Logger.Print('point', 99)



    def spin_once(self):
        if self.__queue.empty():
            # Logger.Debug('gcode_sender  spin_once()')
            return

        # Logger.Print('watting ....', self.__waitting_msg.payload)
        # Logger.Print('rx......', self.__rx.payload)
        if self.__waitting_msg.payload != self.__rx.payload:
            return 

        # queue is not empty, and got last feedback correctly.
        # Logger.Print('queue size', self.__queue.qsize())
        tx_message = self.__queue.get()
        self.__waitting_msg.id = tx_message.id
        self.__waitting_msg.payload = tx_message.payload
        g_mqtt.publish(self.__tx_topic, payload=tx_message.payload)

        print("gcode sender....", self.__tx_topic, self.__waitting_msg.id, self.__waitting_msg.payload)


# Q1: Why do we need so many gcode_senders?
# A1: Because each gcode receiver is a individual CNC,  He has his own gcode queue.
# Q2: Please consider: To use single gcode_sender with many gcode_queues.
# A2a: Yes, We will do that. 
#    The gcode queue is inside gcode_sender now.
#    Step1:  move the gcode_queue out from sender
#    Step2:  Create a pointers wheel, do dispacth gcodes in all queue.
#    Step3:  Make the gcode_sender becomes a SingleTon.
# A2b: No, Not only the gocde_queue, 
#      but also mqtt_topic,  feedback, syncing should be considered for each queue.
#      And, the gcode_sender instance manages queue, feedback, syncing.
#      Maybe a better name is async_gcode_sender ?


all_gcode_senders = []


def gcode_senders_spin_once():
    # Logger.Print("gcode_senders_spin_once()      gcode_sender  count ", len(all_gcode_senders))
    for gcode_sender in all_gcode_senders:
        gcode_sender.spin_once()
