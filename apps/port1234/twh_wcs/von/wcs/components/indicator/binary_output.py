from von.mqtt.remote_var_mqtt import RemoteVar_mqtt
import json


class BinaryOutput:
    def __init__(self) -> None:
        self._state = 'OFF'
    
    def SetState(self, is_turn_on:bool):
        if is_turn_on:
            self._state = 'ON'
        else:
            self._state = 'OFF'

    def GetState(self) -> str:
        return self._state


class BinaryOutputGroup:

    def __init__(self, mqtt_topic:str, group_size:int) -> None:
        self.__leds = list[BinaryOutput]()
        payload = self.__get_mqtt_payload()
        self.__remote_leds = RemoteVar_mqtt(mqtt_topic, json.dumps(payload), False)
        for _ in range(group_size):
            led = BinaryOutput()
            self.__leds.append(led)
    
    def __get_mqtt_payload(self)->str:
        payload = []
        for led in self.__leds:
            payload.append(led.GetState())
        mqtt_payload = json.dumps(payload)
        return mqtt_payload

    def SetState_for_All(self, is_turn_on:bool):
        for led in self.__leds:
            led.SetState(is_turn_on)
        
    def SetState(self, index:int, is_turn_on:bool):
        self.__leds[index].SetState(is_turn_on)
        payload = self.__get_mqtt_payload()
        self.__remote_leds.set(payload)
