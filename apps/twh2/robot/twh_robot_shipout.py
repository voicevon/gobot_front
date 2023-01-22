from von.mqtt_agent import g_mqtt


class TwhRobot_ShipoutBox():
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
        g_mqtt.publish('topic_', self.id, 'green')
    
    def ToTakeout(self) -> None:
        self.state = 'idle'
        g_mqtt.publish('topic', self.id, 'blue')


class TwhRobot_Shipout():
    def __init__(self) -> None:
        self.boxes = [TwhRobot_ShipoutBox(0)]
        for i in range(11):
            newbox = TwhRobot_ShipoutBox(i)
            self.boxes.append(newbox)

    def FindBox_not_fullfilled():
        pass  #???

    def FindBox_Idle(self) -> TwhRobot_ShipoutBox:
        for box in self.boxes:
            if box.state == 'idle':
                return box
        return None

    def FindBox_Feeding(self, order_id: str) -> TwhRobot_ShipoutBox():
        for box in self.boxes:
            if box.order_id == order_id:
                if box.state == 'feeding':
                    return box
        return None

    def FindBox_fullfilled(self, order_id: str) -> TwhRobot_ShipoutBox():
        for box in self.boxes:
            if box.order_id == order_id:
                if box.state == 'fullfilled':
                    return box
        return None



    
