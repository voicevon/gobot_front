

from twh_wcs.von.wcs.porter.loop_porter import LoopPorter
from twh_database.bolt_nut import twh_factories
from von.logger import Logger


class Twh_LoopPorter(LoopPorter):

    def __init__(self, warehouse_id:str, row_id:int) -> None:
        gcode_topic = "twh/" + warehouse_id + '/r' + str(row_id) + "/gcode"  #'twh/221109/r0/gcode'
        self.__state_topic = "twh/" + warehouse_id + '/r' + str(row_id) + "/state"  #'twh/221109/r0/state'
        super().__init__(warehouse_id, row_id, gcode_topic, self.__state_topic)
        self.__target_layer:int

    def _move_to(self, target_col:int, target_layer:int) -> None:
        self.__target_layer = target_layer
        Logger.Info(twh_factories[self.warehouse_id]['name']  + ' -- Twh_LoopPorter::MoveTo()')
        print(  '(row, col, layer) = ' ,self.id, target_col, target_layer )
        
        mcode ='M42P99S1'  # turn off all green leds
        self._gcode_sender.append_gmcode_to_queue(mcode)

        gcode = 'G1X' + str(target_col)
        self._gcode_sender.append_gmcode_to_queue(gcode)

        mcode = 'M408' + self.__state_topic
        self._gcode_sender.append_gmcode_to_queue(mcode)

        mcode ='M999'
        self._gcode_sender.append_gmcode_to_queue(mcode)

    def PickPlace(self, layer:int):
        # move to vertical position
        # gcode = 'G1Z' + str(layer)
        # self._gcode_sender.append_gmcode_to_queue(gcode)

        # push to box together
        # gcode = 'G1P100Q100'
        # self._gcode_sender.append_gmcode_to_queue(gcode)

        # turn on both vaccumm 

        # pull inner nozzle, turn off outter vacuum

        # push inner nozzle

        # pull together

        # turn off inner vacuum

    
        mcode ='M999'
        self._gcode_sender.append_gmcode_to_queue(mcode)

# def ShowLayerLed(self):
    def TurnOn_ItemPickingLed(self, layer:int):
        mcode = 'M42P' + str(self.__target_layer) + 'S1'
        self._gcode_sender.append_gmcode_to_queue(mcode)

        mcode ='M999'
        self._gcode_sender.append_gmcode_to_queue(mcode)

    def _turn_off_leds(self):
        mcode = 'M42P99S1'
        self._gcode_sender.append_gmcode_to_queue(mcode)

        mcode ='M999'
        self._gcode_sender.append_gmcode_to_queue(mcode)  


# twh_loop_porters = list[Twh_LoopPorter]()
