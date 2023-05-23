from twh_wcs.von.wcs.components.binary_output.binary_output_base import BinaryOutput_Basic

from von.mqtt.remote_var_mqtt import RemoteVar_mqtt
from von.logger import Logger
import json



class BinaryOutputGroup:

    def __init__(self, mqtt_topic:str, group_size:int) -> None:
        self.Leds = list[BinaryOutput_Basic]()
        payload = self.__to_mqtt_payload()
        self.__remote_leds = RemoteVar_mqtt(mqtt_topic, json.dumps(payload), False)
        for _ in range(group_size):
            led = BinaryOutput_Basic()
            self.Leds.append(led)

        self.__previous_payload = []
        self.__mqtt_topic = mqtt_topic
    
    def __to_mqtt_payload(self)->str:
        payload = []
        for led in self.Leds:
            payload.append(led.GetState())
        mqtt_payload = json.dumps(payload)
        return mqtt_payload

    def SetState_for_All(self, is_turn_on:bool):
        for led in self.Leds:
            led._set_state(is_turn_on)
        payload = self.__to_mqtt_payload()
        self.__remote_leds.set(payload)
        
        
    def SetElementState(self, index:int, is_turn_on:bool):
        self.Leds[index]._set_state(is_turn_on)
        payload = self.__to_mqtt_payload()
        self.__remote_leds.set(payload)

    def SpinOnce(self):
        payload = self.__to_mqtt_payload()
        if self.__previous_payload == payload:
            return
        Logger.Debug("BinaryOutputGroup::SpinOnce()      publishing")

        Logger.Print(self.__mqtt_topic, payload)
        self.__previous_payload = payload
        self.__remote_leds.set(payload)
