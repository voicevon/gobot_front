
from von.mqtt.remote_var_mqtt import RemoteVar_mqtt

class Wcs_ButtonBase:
    
    def __init__(self, id:str, mqtt_topic:str, on_received_callback) -> None:
        self.__on_received_callback = on_received_callback
        self.__remote_led = RemoteVar_mqtt(mqtt_topic, 'OFF', False)
        self.id = id

    def SpinOnce(self):
        mqtt_payload, has_been_updated = self.__remote_led.get()
        if has_been_updated:
            self.__on_received_callback(self.id, mqtt_payload)
