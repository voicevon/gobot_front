from twh_wcs.twh_robot.twh_loop_porter import Twh_LoopPorter
# from twh_wcs.twh_robot.twh_thames_bridge_packer import Twh_ThamesBridge_Packer
from twh_wcs.von.wcs.deck.simple_deck import SimpleDeck, Wcs_DeckBase

from twh_wcs.von.wcs.workers.pick_placer.manual_pick_placer import Manual_PickPlacer
from twh_wcs.von.wcs.workers.shipper.manual_shipper import Manual_Shipper
# from twh_wcs.von.wcs.gcode_sender import GcodeSender

from twh_wcs.von.wcs.conveyor.tube_conveyor import TubeConveyor
from twh_wcs.von.wcs.workers.porter.loop_porter import LoopPorter
from twh_wcs.von.wcs.workers.pick_placer.pick_placer import Wsc_PickPlacerBase
from twh_wcs.von.wcs.workers.shipper.shipper import Wcs_ShipperBase
from twh_wcs.von.wcs.components.button.button import Wcs_ButtonBase
from twh_wcs.von.wcs.components.indicator.indicator import Wcs_IndicatorBase
from twh_wcs.von.wcs.components.actuator.actuator import Wcs_3wayValve, Wcs_ActuatorBase

from von.logger import Logger



class WcsWorkers:
    warehouse_name = 'not named warehouse'
    
    # complex workers
    loop_porters = list[LoopPorter]()
    tube_conveyors = list[TubeConveyor]()
    pick_placers = list[Wsc_PickPlacerBase]()
    shippers = list[Wcs_ShipperBase]()
    decks = dict[str, list[Wcs_DeckBase]]()


g_workers = dict[str, WcsWorkers]()

class WorkersFactory:

    @classmethod
    def FindIdlePackers(cls, warehouse_id:str, group_name:str) -> list[Wcs_DeckBase]:
        idle_packers = list[Wcs_DeckBase]()
        for packer in g_workers[warehouse_id].decks[group_name]:
            if packer.GetState() == 'idle':
                idle_packers.append(packer)
        return idle_packers

    @classmethod
    def EachWorker_SpinOnce(cls):
        for workers in g_workers.values():
            for porter in workers.loop_porters:
                porter.SpinOnce()
            # for gcode_sender in workers.gcode_senders:
            #     gcode_sender.SpinOnce()
                
    @classmethod
    def Create_WcsWorkers(cls, warehouse_id:str) -> WcsWorkers:
        new_workers = WcsWorkers()

        if warehouse_id == '221109':
            new_workers.warehouse_name = '某某义齿加工厂'

            for loop_porter_index in range(4):
                # new_workers.indicators['loop_porter_layer'] = leds
                new_porter = Twh_LoopPorter(warehouse_id, loop_porter_index)
                new_workers.loop_porters.append(new_porter)
            
            new_picker = Manual_PickPlacer("twh/" + warehouse_id + 'picker')
            new_workers.pick_placers.append(new_picker)

            # packer_decks
            packer_decks = list[Wcs_DeckBase]()
            for i in range(12):
                new_packer = SimpleDeck(i)
                packer_decks.append(new_packer)
            new_workers.decks['packer'] = packer_decks

            # shipper
            new_shipper = Manual_Shipper("twh/" + warehouse_id + 'shipper/button')
            new_workers.shippers.append(new_shipper)

            

            g_workers[warehouse_id] = new_workers
            return new_workers

        elif warehouse_id == '230220':
            g_workers[warehouse_id] = new_workers
            new_workers.warehouse_name = '山东雅乐福义齿加工厂'

            for porter_index in range(1):
                # leds = list[Wcs_IndicatorBase]()
                # for led_index in range(7):
                #     new_led = Wcs_IndicatorBase(led_index)
                #     leds.append(new_led)
                # new_workers.indicators['loop_porter_layer'] = leds
                new_porter = Twh_LoopPorter(warehouse_id, porter_index)
                new_workers.loop_porters.append(new_porter)

            for tuber_index in range(1):
                new_tube_conveyor = TubeConveyor(warehouse_id, 0 ,'','')
                new_workers.tube_conveyors.append(new_tube_conveyor)
            return new_workers
        

        else:
            Logger.Error("CreateWcsInstance()  Error")
            Logger.Print('wcs_instance_id', warehouse_id)
            return None # type: ignore