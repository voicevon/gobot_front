

from twh_wcs.von.wcs.workers.porter.loop_porter import LoopPorter
from twh_wcs.von.wcs.components.indicator.mono_light import MonoLight
from twh_wcs.von.wcs.components.indicator.indicator import Wcs_IndicatorBase
from twh_wcs.wcs_component_factory import ComponentFactory, g_components
from von.logger import Logger


class Twh_LoopPorter(LoopPorter):

    def __init__(self, warehouse_id:str, row_id:int) -> None:
    # def __init__(self, warehouse_id:str, row_id:int, first_led:Wcs_IndicatorBase) -> None:
        '''
        Q: Why do we put led here?
        A: Currently, the led is controlled by Mcode, not mqtt.
           In future, led will be linked to mqtt, then, we can remove parameter: fisrt_led.
        '''
        gcode_topic = "wh" + warehouse_id + '/porter' + str(row_id) + "/gcode"  #'wh221109/porter0/gcode'
        self.__state_topic = "wh" + warehouse_id + '/porter' + str(row_id) + "/state"  #'wh221109/porter0/state'
        super().__init__(warehouse_id, row_id, gcode_topic, self.__state_topic)
        self.__target_layer:int

        self.__leds = list[Wcs_IndicatorBase]()
        for i in range(7):
            mqtt_topic = "wh" + warehouse_id + '/porter' + str(row_id) + '/led' + str(i)   # 'wh221109/porter0/led0'
            led = MonoLight(i, mqtt_topic,self.__event_led_received)
            self.__leds.append(led)
        g_components[self.warehouse_id].indicators['layer_led'] = self.__leds
        
    def __event_led_received(self, index, mqtt_payload):
        # turn on led, turn off led
        pass

    def _move_to(self, target_col:int, target_layer:int) -> None:
        self.__target_layer = target_layer
        # Logger.Info(twh_factories[self.warehouse_id]['name']  + ' -- Twh_LoopPorter::MoveTo()')
        # print(  '(row, col, layer) = ' ,self.id, target_col, target_layer )
        
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
