
from twh_wcs.von.wcs.gcode_sender import GcodeSender, g_gcode_senders
from von.mqtt.remote_var_mqtt import RemoteVar_mqtt
from twh_wcs.von.wcs.worker_base import Wcs_WorkerBase
from von.logger import Logger
from abc import ABC, abstractmethod


# class Wcs_PorterBaseA(ABC, Wcs_WorkerBase):
#     def __init__(self, owner_id: str) -> None:
#         super().__init__(owner_id)

class Wcs_PorterBase(Wcs_WorkerBase):
    
    def __init__(self, warehouse_id:str, row_id:int, gcode_topic, state_topic) -> None:
        '''
        Q: What is default value of state_topic is not 'idle'?
        A: Don't know,  Currently, all requiemnets is satisfied.
        '''
        super().__init__(warehouse_id)
        self.id = row_id
        # self._state = 'idle'
        self._state = RemoteVar_mqtt(state_topic, 'idle')

        self._gcode_sender = GcodeSender(gcode_topic)
        g_gcode_senders.append(self._gcode_sender)

    # def SetStateTo(self, new_state:str):
    #     if new_state in ['idle', 'moving','ready']:
    #         self._state.set(new_state)
    #     else:
    #         Logger.Error("Wcs_PorterBase:: SetStateTo()")
    #         Logger.Print('new_state', new_state)
            
    
    def GetState(self) -> str:
        mqtt_payload, has_been_updated =  self._state.get()
        return mqtt_payload
        # return self._state

    def ResetStatemachine(self):
        if self._state.get() == 'ready':
            self._state.set('idle')
        else:
            Logger.Error('LoopPorterBase  ResetState()')
    

    def Start_CarryToGate(self, bay_carrier_id: int, layer_id: int):
        if self._state.get() == 'idle':
            self._move_to(bay_carrier_id, layer_id)
            self._state.set('moving')
        else:
            Logger.Error('LoopPorterBase  _MoveTo()')

    @abstractmethod
    def _show_layer_led(self):
        pass
        
    @abstractmethod
    def _turn_off_leds(self):
        pass

    @abstractmethod
    def _move_to(self, bay_carrier_id: int, layer_id: int):
        pass

    @abstractmethod
    def PickPlace(self, layer:int):
        pass


