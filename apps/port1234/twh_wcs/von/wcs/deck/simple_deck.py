
from twh_wcs.von.wcs.deck.deck import Wcs_DeckBase
from von.mqtt.mqtt_agent import g_mqtt

'''
TODO: a better name
'''
class SimpleDeck(Wcs_DeckBase):

    def __init__(self, index:int) -> None:
        super().__init__(index)
        self.__green_led_index = 0
        self.__blue_led_index = 0  # TODO: all together for hardware
        

    def SpinOnce(self):
        pass

    def TurnOn_BeingAssigned_Led(self, index:int):
        '''
        Assigned to an order. although the packer is bland at the time.
        '''
        self.__turn_on_packer_cell_led('green', index)

    def TurnOn_Placing_Led(self, index:int):
        '''
        Place order_item to here.
        '''
        self.__turn_on_packer_cell_led('green', index)

    def TurnOn_Fullfilled_Led(self, index:int):
        '''
        Fullfilled, but still being occupied.
        '''
        pass        

    def __turn_on_packer_cell_led(self, color:str, cell_id: int):
        if color == 'green':
            self.__green_led_index = cell_id
        if color == 'blue':
            self.__blue_led_index = cell_id

        topic = 'twh/221109/packer/led'

        payload =  str(self.__green_led_index + 100* self.__blue_led_index)
        g_mqtt.publish(topic, payload)
