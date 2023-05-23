from twh_wcs.twh_robot.twh_loop_porter import Twh_LoopPorter
# from twh_wcs.twh_robot.twh_thames_bridge_packer import Twh_ThamesBridge_Packer
from twh_wcs.von.wcs.workers.pick_placer.manual_pick_placer import Manual_PickPlacer
from twh_wcs.von.wcs.workers.shipper.manual_shipper import Manual_Shipper
from twh_wcs.von.wcs.conveyor.tube_conveyor import TubeConveyor
from twh_wcs.von.wcs.workers.wcs_item_workers import WcsWorkers

from twh_wcs.wcs_component_factory import ComponentFactory
from twh_wcs.wcs_warehouse_factory import g_warehouses

from von.logger import Logger

from twh_wcs.wcs_warehouse_factory import g_warehouses

# g_workers = dict[str, WcsWorkers]()


class WorkersFactory:

    @classmethod
    def Create_WcsWorkers(cls, warehouse_id:str) -> WcsWorkers:
        new_workers = WcsWorkers()
        # new_workers = g_warehouses[warehouse_id].workers_take
        # g_components[warehouse_id] = ComponentFactory.CreateBlankComponents()
        # g_warehouses[warehouse_id].components_take
        if warehouse_id == '221109':
            for loop_porter_index in range(4):
                # new_workers.indicators['loop_porter_layer'] = leds
                new_porter = Twh_LoopPorter(warehouse_id, loop_porter_index)
                new_workers.loop_porters.append(new_porter)
            
            new_picker = Manual_PickPlacer('picker')
            new_workers.pick_placers.append(new_picker)

            # shipper
            new_shipper = Manual_Shipper("twh/" + warehouse_id + 'shipper/button')
            new_workers.shippers.append(new_shipper)

            g_warehouses[warehouse_id].workers_take = new_workers
            return new_workers

        elif warehouse_id == '230220':
            new_workers = WcsWorkers()
            for porter_index in range(1):
                new_porter = Twh_LoopPorter(warehouse_id, porter_index)
                new_workers.loop_porters.append(new_porter)

            for tuber_index in range(1):
                new_tube_conveyor = TubeConveyor(warehouse_id, 0 ,'','')
                new_workers.tube_conveyors.append(new_tube_conveyor)

            g_warehouses[warehouse_id].workers_take = new_workers
            return new_workers
        

        else:
            Logger.Error("CreateWcsInstance()  Error")
            Logger.Print('wcs_instance_id', warehouse_id)
            return None # type: ignore