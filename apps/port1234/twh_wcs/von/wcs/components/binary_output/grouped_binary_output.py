from twh_wcs.von.wcs.components.binary_output.binary_output_base import BinaryOutput_Basic

from von.mqtt.remote_var_mqtt import RemoteVar_mqtt
import json


class BinaryOutputGroup:

    def __init__(self, mqtt_topic:str, group_size:int) -> None:
        self.__leds = list[BinaryOutput_Basic]()
        payload = self.__to_mqtt_payload()
        self.__remote_leds = RemoteVar_mqtt(mqtt_topic, json.dumps(payload), False)
        for _ in range(group_size):
            led = BinaryOutput_Basic()
            self.__leds.append(led)
    
    def __to_mqtt_payload(self)->str:
        payload = []
        for led in self.__leds:
            payload.append(led.GetState())
        mqtt_payload = json.dumps(payload)
        return mqtt_payload

    def SetState_for_All(self, is_turn_on:bool):
        for led in self.__leds:
            led._set_state(is_turn_on)
        payload = self.__to_mqtt_payload()
        self.__remote_leds.set(payload)
        
        
    def SetState(self, index:int, is_turn_on:bool):
        self.__leds[index]._set_state(is_turn_on)
        payload = self.__to_mqtt_payload()
        self.__remote_leds.set(payload)
