from twh_wcs.von.wcs.components.binary_output.binary_output_base import BinaryOutput_Basic

from von.mqtt.remote_var_mqtt import RemoteVar_mqtt


class SingleBinaryOutput(BinaryOutput_Basic):

    def __init__(self, mqtt_topic:str) -> None:
        super().__init__()
        self.__remote = RemoteVar_mqtt(mqtt_topic, self._state, False)
    
    def SetState(self, is_turn_on:bool):
        self._set_state(is_turn_on)
        self.__remote.set(self._state)