from logger import Logger
from von.mqtt_agent import g_mqtt

class TwhRobot_PackerCell():

    def __init__(self, id:int) -> None:
        #  'idle', 'feeding', 'fullfilled', 'packing'
        self.__state = 'idle' 
        self.id = id   #range is [0,11]
        self.order_id = None

    def SetStateTo(self, state:str):
        self.__state = state

    def GetState(self) -> str:
        return self.__state
    
    def PrintOut(self, title):
        Logger.Info(title)
        Logger.Print("id" ,self.id)
        Logger.Print('order_id' , self.order_id)
        Logger.Print('__state' , self.__state)

class TwhRobot_Packer():
    def __init__(self) -> None:
        self.__green_led_index = 13
        self.__blue_led_index = 13
        self.__cells = [TwhRobot_PackerCell(0)]
        for i in range(11):
            newbox = TwhRobot_PackerCell(i+1)
            self.__cells.append(newbox)

    def SetPackingCell(self, cell_id:int):
        '''
        cell_id == -1, means no packing cell
        '''
        self.__packing_cell_id = cell_id

    def GetPackingCell_id(self) ->int:
        return self.__packing_cell_id

    def GetCell(self, cell_id:int) -> TwhRobot_PackerCell:
        return self.__cells[cell_id]

    def PrintOut(self, title):
        Logger.Info(title)
        for cell in self.__cells:
            cell.PrintOut(title)

    def turn_on_cell_led(self, color:str, cell_id: int):
        if color == 'green':
            self.__green_led_index = cell_id
        if color == 'blue':
            self.__blue_led_index = cell_id

        topic = 'twh/221109/packer/led'
        payload =  self.__green_led_index + 100* self.__blue_led_index
        g_mqtt.publish(topic, payload)

    def turn_off_all_led(self, color:str):
        topic = 'twh/221109/packer/led'
        payload = 1313
        g_mqtt.publish(topic, payload)
        

    # def CheckMqttMessage(self):
    #     print("TwhRobot_Shipout main process is started....")
        # rx = g_mqtt.RxBuffer
        # if rx.has_updated_payload(self.rx_topic):
        #     payload = rx.FetchPayload(self.rx_topic)
        #     box_id =   payload['box_id'] 
        #     box = self.__cells[box_id]
        #     box.state = payload['state']

    def Find_IdleCell(self) -> TwhRobot_PackerCell:
        for cell in self.__cells:
            if cell.GetState() == 'idle':
                return cell
        return None

    def Find_FeedingCell(self, order_id: str) -> TwhRobot_PackerCell:
        for cell in self.__cells:
            if cell.order_id == order_id:
                if cell.GetState() == 'feeding':
                    return cell
        return None

    def Find_fullfilledCell(self, order_id: str) -> TwhRobot_PackerCell:
        for cell in self.__cells:
            if cell.order_id == order_id:
                if cell.GetState() == 'fullfilled':
                    return cell
        return None

    

