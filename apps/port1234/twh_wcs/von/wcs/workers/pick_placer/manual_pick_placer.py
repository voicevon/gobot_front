from twh_wcs.von.wcs.workers.pick_placer.pick_placer import Wsc_PickPlacerBase
from twh_wcs.wcs_component_factory import g_components
# from twh_wcs.von.wcs.deck.simple_deck import SimpleDeck
# from von.mqtt.remote_var_mqtt import RemoteVar_mqtt

class Manual_PickPlacer(Wsc_PickPlacerBase):
    def __init__(self, mqtt_topic_of_placed_button:str) -> None:
        super().__init__()
        self.__button_placed = g_components[self.warehouse_id].buttons['placed']

    # def Start(self, pick_at, place_at:SimpleDeck):
    def Start(self):
        self.__button_placed.Reset()
        self._state = 'picking_placing'

    def SpinOnce(self):
        # mqtt_payload, has_been_updated =  self.__button_placed.get()
        # if mqtt_payload == 'ON':
        if self.__button_placed.IsPressed():
            self._state =  'picked_placed'

