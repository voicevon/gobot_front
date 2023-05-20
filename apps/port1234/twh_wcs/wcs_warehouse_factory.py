
from twh_wcs.twhwcs_loop_manual_pack_system.twhwcs_loop_manual_packer import TwhWcs_LoopManualPacker
from twh_wcs.twhwcs_loop_tube_system.twhwcs_loop_tube import Twh_LoopTubeSystem

from twh_wcs.von.wcs.warehouse_base import WarehouseBase
from von.logger import Logger
import multiprocessing

g_warehouses = dict[str, WarehouseBase]()

class WarehouseFactory:
    @classmethod
    def Create_Warehouse(cls, warehouse_id:str, deposit_queue:multiprocessing.Queue) -> WarehouseBase:

        if warehouse_id == '221109':
            warehouse = TwhWcs_LoopManualPacker(warehouse_id, deposit_queue)
            # wcs_instance = Twh_LoopTubeSystem(wcs_instance_id, deposit_queue)
            g_warehouses[warehouse_id] = warehouse
            return warehouse

        elif warehouse_id == '230220':
            warehouse = Twh_LoopTubeSystem(warehouse_id, deposit_queue)
            g_warehouses[warehouse_id] = warehouse
            return warehouse
        

        else:
            Logger.Error("CreateWcsInstance()  Error")
            Logger.Print('wcs_instance_id', warehouse_id)
            return None # type: ignore