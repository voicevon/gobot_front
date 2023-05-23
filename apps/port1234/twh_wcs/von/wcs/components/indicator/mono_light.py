from twh_wcs.von.wcs.components.indicator.indicator import Wcs_IndicatorBase
from twh_wcs.wcs_component_factory import g_components
from von.mqtt.remote_var_mqtt import RemoteVar_mqtt, g_mqtt
import json

class MonoLight(Wcs_IndicatorBase):

    def __init__(self, index: int, mqtt_topic:str) -> None:
        super().__init__(index)
        self.__remote_led = RemoteVar_mqtt(mqtt_topic, 'OFF', False)

    def GetState(self):
        mqtt_payload, has_been_updated = self.__remote_led.get()
        # if has_been_updated:
        #     self.__on_received_callback(self.index, mqtt_payload)
        return mqtt_payload


        
