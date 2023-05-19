

# from twh_wcs.von.wcs.porter.porter import Wcs_PorterBase
# from twh_wcs.twh_order import Twh_Order, Twh_OrderItem
from twh_wcs.von.wcs.porter.loop_porter import LoopPorter
from twh_database.bolt_nut import twh_factories
from von.logger import Logger


class Twh_LoopPorter(LoopPorter):

    def __init__(self, wcs_unit_id:str, row_id:int) -> None:
        state_topic = "twh/" + wcs_unit_id + '/r' + str(row_id) + "/state"  #'twh/221109/r0/state'
        gcode_topic = "twh/" + wcs_unit_id + '/r' + str(row_id) + "/gcode"  #'twh/221109/r0/gcode'
        super().__init__(wcs_unit_id, row_id, gcode_topic, state_topic)
        self.__target_layer:int

    def _MoveTo(self, target_col:int, target_layer:int) -> None:
        self._state.set('moving')    # set to 'moving' when gcode-G1 is sent. ??
        self.__target_layer = target_layer
        Logger.Info(twh_factories[self.wcs_unit_id]['name']  + ' -- Twh_LoopPorter::MoveTo()')
        print(  '(row, col, layer) = ' ,self.id, target_col, target_layer )
        
        mcode ='M42P99S1'  # turn off all green leds
        self._gcode_sender.append_gmcode_to_queue(mcode)

        gcode = 'G1X' + str(target_col)
        self._gcode_sender.append_gmcode_to_queue(gcode)

        mcode = 'M408' + self._state_topic
        self._gcode_sender.append_gmcode_to_queue(mcode)

        mcode ='M999'
        self._gcode_sender.append_gmcode_to_queue(mcode)

    def show_layer_led(self):
        mcode = 'M42P' + str(self.__target_layer) + 'S1'
        self._gcode_sender.append_gmcode_to_queue(mcode)

        mcode ='M999'
        self._gcode_sender.append_gmcode_to_queue(mcode)

    def turn_off_leds(self):
        mcode = 'M42P99S1'
        self._gcode_sender.append_gmcode_to_queue(mcode)

        mcode ='M999'
        self._gcode_sender.append_gmcode_to_queue(mcode)  



# twh_loop_porters = list[Twh_LoopPorter]()
