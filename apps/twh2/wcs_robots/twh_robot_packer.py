from business_logical.withdraw_order import WithdrawOrder

from logger import Logger
from von.mqtt_agent import g_mqtt
from von.remote_var_mqtt import RemoteVar_mqtt


class TwhRobot_Packer():
    def __init__(self, button_pack:RemoteVar_mqtt) -> None:
        self.__green_led_index = 13
        self.__blue_led_index = 13
        self.__button_pack = button_pack
        self.__shipping_order = None

    def GetShippingOrder(self) -> WithdrawOrder:
        return self.__shipping_order # type: ignore
    
    def SetShippingOrder(self, order:WithdrawOrder, packer_cell_id:int):
        self.__shipping_order = order
        self.__Lock_packer_cell(packer_cell_id)

    def Check_Shipout_button(self):
        if self.__button_pack.get() == 'ON':
            self.turn_off_all_led('blue')
            if self.__shipping_order is None:
                Logger.Error('CheckButton()')
            else:
                self.__shipping_order.SetStateTo('shipped', write_to_db=True)
            self.__shipping_order = None

    def Find_Idle_packer_cell(self)->int:
        '''
        return:
          * -1:  there is no idle packer_cell
        '''
        return 1

    def __Lock_packer_cell(self, packer_cell_id:int):
        pass  

    def Release_packer_cell(self, packer_cell_id:int):
        pass  
    
    def turn_on_cell_led(self, color:str, cell_id: int):
        if color == 'green':
            self.__green_led_index = cell_id
        if color == 'blue':
            self.__blue_led_index = cell_id

        topic = 'twh/221109/packer/led'

        payload =  str(self.__green_led_index + 100* self.__blue_led_index)
        Logger.Debug("turn_on_cell_led()")
        Logger.Print('payload',payload)
        g_mqtt.publish(topic, payload)

    def turn_off_all_led(self, color:str):
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


    

