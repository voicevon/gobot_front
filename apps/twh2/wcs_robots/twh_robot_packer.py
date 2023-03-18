from logger import Logger
# from wcs_robots.gcode_sender import GcodeSender, all_gcode_senders
from von.mqtt_agent import g_mqtt

class TwhRobot_PackBox():

    def __init__(self, id:int) -> None:
        #  'idle', 'feeding', 'fullfilled'
        self.state = 'idle' 
        self.id = id
        self.order_id = None

    def ToFeedIn(self, is_last_tooth: bool) -> None:
        if is_last_tooth:
            self.state = 'fullfilled'
        else:
            self.state = 'feeding'
        # g_mqtt.publish('topic_', self.id, 'green')
    
    def ToTakeout(self) -> None:
        self.state = 'idle'
        # g_mqtt.publish('topic', self.id, 'blue')

    def PrintOut(self, title):
        Logger.Info(title)
        Logger.Print("id" ,self.id)
        Logger.Print('order_id' , self.order_id)
        Logger.Print('state' , self.state)

class TwhRobot_Packer():
    def __init__(self) -> None:
        self.boxes = [TwhRobot_PackBox(0)]
        for i in range(11):
            newbox = TwhRobot_PackBox(i+1)
            self.boxes.append(newbox)
        # self.rx_topic = 'twh/221109/packer/box'

        # gcode_topic = "twh/221109/packer/gcode"
        # self.gcode_sender = GcodeSender(gcode_topic)
        # all_gcode_senders.append(self.gcode_sender)

    def PrintOut(self, title):
        Logger.Info(title)
        for box in self.boxes:
            box.PrintOut('----')

    def show_pack_box_led(self, packbox_id: int):
        # gcode = 'M42P' + str(packbox_id) + 'S1'
        # Logger.Print('TwhRobot_Packer::show_pack_box_led()   ',  gcode)
        # self.gcode_sender.append_gmcode_to_queue(gcode)
        topic = 'twh/221109/packer/led'
        payload = packbox_id
        g_mqtt.publish(topic, payload)
        

    def CheckMqttMessage(self):
        print("TwhRobot_Shipout main process is started....")
        # rx = g_mqtt.RxBuffer
        # if rx.has_updated_payload(self.rx_topic):
        #     payload = rx.FetchPayload(self.rx_topic)
        #     box_id =   payload['box_id'] 
        #     box = self.boxes[box_id]
        #     box.state = payload['state']

    # def FindBox_not_fullfilled():
    #     pass  #???

    def FindBox_Idle(self) -> TwhRobot_PackBox:
        for box in self.boxes:
            if box.state == 'idle':
                return box
        return None

    def FindBox_Feeding(self, order_id: str) -> TwhRobot_PackBox:
        for box in self.boxes:
            if box.order_id == order_id:
                if box.state == 'feeding':
                    return box
        return None

    def FindBox_fullfilled(self, order_id: str) -> TwhRobot_PackBox:
        for box in self.boxes:
            if box.order_id == order_id:
                if box.state == 'fullfilled':
                    return box
        return None

    

