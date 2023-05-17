from wcs_robots.wcs_base.gcode_sender import GcodeSender, all_gcode_senders
from von.mqtt.remote_var_mqtt import RemoteVar_mqtt


class PorterBase:
    
    def __init__(self, wcs_unit_id:str, row_id:int, gcode_topic, state_topic) -> None:
        '''
        Q: What is default value of state_topic is not 'idle'?
        A: Don't know,  Currently, all requiemnets is satisfied.
        '''
        self.id = row_id
        self.wcs_unit_id = wcs_unit_id
        self._gcode_sender = GcodeSender(gcode_topic)
        all_gcode_senders.append(self._gcode_sender)
        self._state = RemoteVar_mqtt(state_topic, 'idle')
        self._state_topic = state_topic

    def SetStateTo(self, new_state:str):
        self._state.set(new_state)
    
    def GetState(self) -> str:
        mqtt_payload, has_been_updated =  self._state.get()
        return mqtt_payload

