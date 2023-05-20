from twh_wcs.von.wcs.pick_placer.pick_placer import Wsc_PickPlacerBase
from von.mqtt.remote_var_mqtt import RemoteVar_mqtt

from twh_wcs.von.wcs.gcode_sender import GcodeSender, g_gcode_senders

class ZPQ_PickPlacer(Wsc_PickPlacerBase):
    def __init__(self) -> None:
        super().__init__()
        self.__gcode_sender = GcodeSender('abcdef')
        g_gcode_senders.append(self.__gcode_sender)

    def start_pick_placing(self):
        pass
    
    def Start(self):
        gcode = "app:pick"
        self.__gcode_sender.append_gmcode_to_queue(gcode)
        self._state ='picking'
        
    def SpinOnce(self):
        if self._state == 'picking':
            self._state = 'placed'
    

