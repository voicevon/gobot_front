
from twh_wcs.warehouse_loop_manual_pack.pastor_loop_manual_packer import Pastor_LoopManualPacker
from twh_wcs.warehouse_loop_tube.pastor_loop_tube import Twh_LoopTubeSystem



from twh_wcs.von.wcs.pastor_base import PastorBase
from von.logger import Logger
import multiprocessing

g_pastors = dict[str, PastorBase]()

class PastorFactory:

    @classmethod
    def EachPastor_SpinOnce(cls):
        for pastor in g_pastors.values():
            pastor.SpinOnce()

    @classmethod
    def Create_Pastor(cls, warehouse_id:str, deposit_queue:multiprocessing.Queue) -> PastorBase:
        if warehouse_id == '221109':
            pastor = Pastor_LoopManualPacker(warehouse_id, deposit_queue)
            g_pastors[warehouse_id] = pastor
            return pastor

        elif warehouse_id == '230220':
            pastor = Twh_LoopTubeSystem(warehouse_id, deposit_queue)
            g_pastors[warehouse_id] = pastor
            return pastor

        else:
            Logger.Error("CreateWcsInstance()  Error")
            Logger.Print('wcs_instance_id', warehouse_id)
            return None # type: ignore