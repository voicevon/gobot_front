from twh_wcs.twh_robot.twh_loop_porter import Twh_LoopPorter
from twh_wcs.von.wcs.workers.pick_placer.manual_pick_placer import Manual_PickPlacer
from twh_wcs.von.wcs.workers.shipper.manual_shipper import Manual_Shipper
from twh_wcs.von.wcs.workers.conveyor.simple_tube import SimpleTubeConveyor
from twh_wcs.von.wcs.workers.wcs_item_workers import WcsWorkers

from twh_wcs.wcs_warehouse_factory import g_warehouses

from von.logger import Logger

from twh_wcs.wcs_warehouse_factory import g_warehouses


class WorkersFactory:

    @classmethod
    def Create_WcsWorkers(cls, warehouse_id:str) -> WcsWorkers:
        new_workers = WcsWorkers()
        if warehouse_id == '221109':
            for loop_porter_index in range(4):
                new_porter = Twh_LoopPorter(warehouse_id, loop_porter_index)
                new_workers.loop_porters.append(new_porter)
            
            new_picker = Manual_PickPlacer(warehouse_id)
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
                new_tube_conveyor = SimpleTubeConveyor(warehouse_id)
                new_workers.tube_conveyors.append(new_tube_conveyor)

            g_warehouses[warehouse_id].workers_take = new_workers
            return new_workers
        

        else:
            Logger.Error("CreateWcsInstance()  Error")
            Logger.Print('wcs_instance_id', warehouse_id)
            return None # type: ignore