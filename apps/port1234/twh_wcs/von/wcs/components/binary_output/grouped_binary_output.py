from twh_wcs.von.wcs.components.binary_output.binary_output_base import BinaryOutput_Basic

from von.mqtt.remote_var_mqtt import RemoteVar_mqtt
from von.logger import Logger
import json



class BinaryOutputGroup:

    def __init__(self, mqtt_topic:str, group_size:int) -> None:
        self.Gates = list[BinaryOutput_Basic]()
        for _ in range(group_size):
            led = BinaryOutput_Basic()
            self.Gates.append(led)

        # now, the list is not blank.
        self.__previous_mqtt_payload = self.__to_mqtt_payload()
        self.__remote_leds = RemoteVar_mqtt(mqtt_topic, self.__previous_mqtt_payload, for_loading_config=False)
        self.__mqtt_topic = mqtt_topic
    
    def __to_mqtt_payload(self)->str:
        payload = []
        if len(self.Gates) == 0:
            Logger.Error("BinaryOutputGroup::__to_mqtt_payload()")
            while True:
                pass
            
        for led in self.Gates:
            payload.append(led.GetState())
        mqtt_payload = json.dumps(payload)
        return mqtt_payload

    def SetState_for_All(self, is_turn_on:bool):
        for led in self.Gates:
            led._set_state(is_turn_on)
        mqtt_payload = self.__to_mqtt_payload()
        self.__remote_leds.set(mqtt_payload)
        
    def SetElementState(self, index:int, is_turn_on:bool):
        self.Gates[index]._set_state(is_turn_on)
        payload = self.__to_mqtt_payload()
        self.__remote_leds.set(payload)

    def SpinOnce(self):
        mqtt_payload = self.__to_mqtt_payload()
        if self.__previous_mqtt_payload == mqtt_payload:
            return
        Logger.Debug("BinaryOutputGroup::SpinOnce()      publishing")

        Logger.Print(self.__mqtt_topic, mqtt_payload)
        self.__previous_mqtt_payload = mqtt_payload
        self.__remote_leds.set(mqtt_payload)
