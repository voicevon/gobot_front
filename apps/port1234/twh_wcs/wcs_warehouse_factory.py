
from twh_wcs.warehouse_loop_manual_pack.warehouse_loop_manual_packer import Warehosue_LoopManualPacker
from twh_wcs.warehouse_loop_tube.warehosue_loop_tube import Twh_LoopTubeSystem



from twh_wcs.von.wcs.warehouse_base import WarehouseBrainBase
from von.logger import Logger
import multiprocessing

g_brains = dict[str, WarehouseBrainBase]()

class WarehouseFactory:

    @classmethod
    def EachWarehouse_SpinOnce(cls):
        for w in g_brains.values():
            w.SpinOnce()
        

    @classmethod
    def Create_Warehouse(cls, warehouse_id:str, deposit_queue:multiprocessing.Queue) -> WarehouseBrainBase:

        if warehouse_id == '221109':
            warehouse = Warehosue_LoopManualPacker(warehouse_id, deposit_queue)
            # wcs_instance = Twh_LoopTubeSystem(wcs_instance_id, deposit_queue)
            g_brains[warehouse_id] = warehouse
            return warehouse

        elif warehouse_id == '230220':
            warehouse = Twh_LoopTubeSystem(warehouse_id, deposit_queue)
            g_brains[warehouse_id] = warehouse
            return warehouse
        

        else:
            Logger.Error("CreateWcsInstance()  Error")
            Logger.Print('wcs_instance_id', warehouse_id)
            return None # type: ignore