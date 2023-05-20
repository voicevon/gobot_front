from twh_wcs.von.wcs.pick_placer.pick_placer import Wsc_PickPlacerBase
from von.mqtt.remote_var_mqtt import RemoteVar_mqtt

class Manual_PickPlacer(Wsc_PickPlacerBase):
    def __init__(self, mqtt_topic_of_placed_button:str) -> None:
        super().__init__()
        self.__button_placed = RemoteVar_mqtt(mqtt_topic_of_placed_button, 'OFF')

    def Start(self, pick_at, place_at):
        # turn on led pair 
        pass

    def SpinOnce(self):
        mqtt_payload, has_been_updated =  self.__button_placed.get()
        self._state =  mqtt_payload

