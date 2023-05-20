
from twh_wcs.twhwcs_loop_manual_pack_system.twhwcs_loop_manual_packer import TwhWcs_LoopManualPacker
from twh_wcs.twhwcs_loop_tube_system.twhwcs_loop_tube import Twh_LoopTubeSystem

import multiprocessing

from twh_wcs.von.wcs.warehouse_base import WarehouseBase

from von.logger import Logger

g_warehouses = dict[str, WarehouseBase]()

def Create_Warehouse(warehouse_id:str, deposit_queue:multiprocessing.Queue) -> WarehouseBase:

    if warehouse_id == '221109':
        wcs_instance = TwhWcs_LoopManualPacker(warehouse_id, deposit_queue)
        # wcs_instance = Twh_LoopTubeSystem(wcs_instance_id, deposit_queue)
        g_warehouses[warehouse_id] = wcs_instance
        return wcs_instance

    elif warehouse_id == '230220':
        wcs_instance = Twh_LoopTubeSystem(warehouse_id, deposit_queue)
        g_warehouses[warehouse_id] = wcs_instance
        return wcs_instance
    

    else:
        Logger.Error("CreateWcsInstance()  Error")
        Logger.Print('wcs_instance_id', wcs_instance_id)
        return None # type: ignore