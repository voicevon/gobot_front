
from twh_wcs.von.wcs.order import Wcs_OrderBase, Wcs_OrderItemBase
from twh_wcs.von.wcs.gcode_sender import GcodeSender, g_gcode_senders
from von.mqtt.remote_var_mqtt import RemoteVar_mqtt

from von.logger import Logger
from abc import ABC, abstractmethod


class Wcs_PorterBase(ABC):
    
    def __init__(self, wcs_unit_id:str, row_id:int, gcode_topic, state_topic) -> None:
        '''
        Q: What is default value of state_topic is not 'idle'?
        A: Don't know,  Currently, all requiemnets is satisfied.
        '''
        self.id = row_id
        self.wcs_unit_id = wcs_unit_id
        self._gcode_sender = GcodeSender(gcode_topic)
        g_gcode_senders.append(self._gcode_sender)
        self._state = RemoteVar_mqtt(state_topic, 'idle')
        self._state_topic = state_topic

    def SetStateTo(self, new_state:str):
        if new_state in ['idle', 'moving']:
            self._state.set(new_state)
        else:
            Logger.Error("Wcs_PorterBase:: SetStateTo()")
            Logger.Print('new_state', new_state)
            
    
    def GetState(self) -> str:
        mqtt_payload, has_been_updated =  self._state.get()
        return mqtt_payload

    @abstractmethod
    def PickPlace(self, layer:int):
        pass

    @abstractmethod
    def _MoveTo(self, target_col:int, target_layer:int) -> None:
        pass

    @abstractmethod
    def _show_layer_led(self):
        pass
        
    @abstractmethod
    def _turn_off_leds(self):
        pass

    # def CarryToGate(self, order: Wcs_OrderBase, order_item:Wcs_OrderItemBase):
    #     self.__porting_order = order
    #     self.__porting_item = order_item
    #     # Carry porting_item to gate
    #     gate_at = self._GetGateLocation()
        # self.MoveTo(gate_at.col, gate_at.layer)

    def CarryToGate(self, bay_carrier_id: int, layer_id: int):
        self._MoveTo(bay_carrier_id, layer_id)

    # def Get_Porting_Order_and_Item(self) -> tuple[Wcs_OrderBase, Wcs_OrderItemBase]:
    #     return  (self.__porting_order, self.__porting_item)
    




