# from business_logical.withdraw_order import WithdrawOrder

from logger import Logger
from von.mqtt_agent import g_mqtt


class TwhRobot_Packer():
    def __init__(self) -> None:
        self.__green_led_index = 13
        self.__blue_led_index = 13
        # self.__shipping_order_id = 0
        self.__packer_cells_state = [0,0,0,0, 0,0,0,0, 0,0,0,0]

    # def Get_ShippingOrder_id(self) -> int:
    #     return self.__shipping_order_id # type: ignore
    
    # def SetShippingOrder(self, packer_cell_id:int):
    def StartShipping(self, packer_cell_id:int):
        self.__turn_on_packer_cell_led('blue',packer_cell_id)

    def StartFeeding_LockPackerCell(self, packer_cell_id:int):
        self.__packer_cells_state[packer_cell_id] = 1
        
  
    def Find_Idle_packer_cell(self)->int:
        '''
        return:
          * -1:  there is no idle packer_cell
        '''
        for index  in  range(12):
            if self.__packer_cells_state[index] == 0:
                return index
        return -1

    # def Lock_packer_cell(self, packer_cell_id:int):
    #     self.__packer_cells_state[packer_cell_id] = 1

    def Release_packer_cell(self, packer_cell_id:int):
        '''
        Release a packer_cell, because the shipping order is shipped
        '''
        # self.__shipping_order_id = 0
        self.__packer_cells_state[packer_cell_id] = 0
        self.__turn_off_all_packer_cells_led('blue')

    def turn_on_packer_cell_led_green(self, cell_id:int):
        self.__turn_on_packer_cell_led('green', cell_id)

    def turn_off_all_packer_cells_led_green(self):
        self.__turn_off_all_packer_cells_led('green')

    def turn_off_all_packer_cells_led_blue(self):
        self.__turn_off_all_packer_cells_led('blue')

    def __turn_on_packer_cell_led(self, color:str, cell_id: int):
        if color == 'green':
            self.__green_led_index = cell_id
        if color == 'blue':
            self.__blue_led_index = cell_id

        topic = 'twh/221109/packer/led'

        payload =  str(self.__green_led_index + 100* self.__blue_led_index)
        Logger.Debug("TwhRobot_Packer:: turn_on_packer_cell_led()")
        Logger.Print('payload',payload)
        g_mqtt.publish(topic, payload)

    def __turn_off_all_packer_cells_led(self, color:str):
        topic = 'twh/221109/packer/led'
        if color == 'green':
            self.__green_led_index = 13
        if color == 'blue':
            self.__blue_led_index = 13

        payload =  self.__green_led_index + 100* self.__blue_led_index
        g_mqtt.publish(topic, payload)
        

    # def Find_IdleCell(self) -> TwhRobot_PackerCell:
    #     for cell in self.__cells:
    #         if cell.GetState() == 'idle':
    #             return cell
    #     return None # type: ignore


    

