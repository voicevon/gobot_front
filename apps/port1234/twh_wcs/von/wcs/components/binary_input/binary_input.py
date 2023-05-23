
from von.mqtt.remote_var_mqtt import RemoteVar_mqtt

class Wcs_ButtonBase:
    
    def __init__(self, mqtt_topic:str, on_received_callback=None) -> None:
        self.__on_received_callback = on_received_callback
        self.__remote_button_state = RemoteVar_mqtt(mqtt_topic, 'OFF', False)

    def SetOnReceived_Callback(self, on_received_callback):
        self.__on_received_callback = on_received_callback

    def Reset(self):
        self.__remote_button_state.set('OFF')

    
    def IsPressed(self) -> bool:
        mqtt_payload, has_been_updated = self.__remote_button_state.get()
        if has_been_updated:
            if mqtt_payload == 'ON':
                self.Reset()
                return True
        return False
    
    # def SpinOnce(self):
    #     mqtt_payload, has_been_updated = self.__remote_led.get()
    #     if has_been_updated:
    #         if self.__on_received_callback is not None:
    #             self.__on_received_callback(self.id, mqtt_payload)
