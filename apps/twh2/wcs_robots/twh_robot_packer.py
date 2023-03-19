from logger import Logger
from von.mqtt_agent import g_mqtt

class TwhRobot_PackingCell():

    def __init__(self, id:int) -> None:
        #  'idle', 'feeding', 'fullfilled'
        self.state = 'idle' 
        self.id = id
        self.order_id = None

    def PrintOut(self, title):
        Logger.Info(title)
        Logger.Print("id" ,self.id)
        Logger.Print('order_id' , self.order_id)
        Logger.Print('state' , self.state)

class TwhRobot_Packer():
    def __init__(self) -> None:
        self.boxes = [TwhRobot_PackingCell(0)]
        for i in range(11):
            newbox = TwhRobot_PackingCell(i+1)
            self.boxes.append(newbox)

    def PrintOut(self, title):
        Logger.Info(title)
        for box in self.boxes:
            box.PrintOut('----')

    def show_cell_led(self, color:str, packbox_id: int):
        topic = 'twh/221109/packer/led'
        payload = packbox_id
        if color =='green':
            payload += 12
        if color == 'blue':
            payload += 2 * 12
        g_mqtt.publish(topic, payload)

    def turn_off_all_led(self, color:str):
        topic = 'twh/221109/packer/led'
        payload = 111
        g_mqtt.publish(topic, payload)
        

    def CheckMqttMessage(self):
        print("TwhRobot_Shipout main process is started....")
        # rx = g_mqtt.RxBuffer
        # if rx.has_updated_payload(self.rx_topic):
        #     payload = rx.FetchPayload(self.rx_topic)
        #     box_id =   payload['box_id'] 
        #     box = self.boxes[box_id]
        #     box.state = payload['state']

    def FindBox_Idle(self) -> TwhRobot_PackingCell:
        for box in self.boxes:
            if box.state == 'idle':
                return box
        return None

    def FindBox_Feeding(self, order_id: str) -> TwhRobot_PackingCell:
        for box in self.boxes:
            if box.order_id == order_id:
                if box.state == 'feeding':
                    return box
        return None

    def FindBox_fullfilled(self, order_id: str) -> TwhRobot_PackingCell:
        for box in self.boxes:
            if box.order_id == order_id:
                if box.state == 'fullfilled':
                    return box
        return None

    

