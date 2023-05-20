
from twh_wcs.twhwcs_loop_manual_pack_system.twhwcs_loop_manual_packer import TwhWcs_LoopManualPacker
from twh_wcs.twhwcs_loop_tube_system.twhwcs_loop_tube import Twh_LoopTubeSystem

import multiprocessing

from twh_wcs.von.wcs.wcs_instance_base import Wcs_InstanceBase

from von.logger import Logger

g_leaders = dict[str, Wcs_InstanceBase]()

def Create_WcsLeader(wcs_instance_id:str, deposit_queue:multiprocessing.Queue) -> Wcs_InstanceBase:

    if wcs_instance_id == '221109':
        wcs_instance = TwhWcs_LoopManualPacker(wcs_instance_id, deposit_queue)
        # wcs_instance = Twh_LoopTubeSystem(wcs_instance_id, deposit_queue)
        g_leaders[wcs_instance_id] = wcs_instance
        return wcs_instance

    elif wcs_instance_id == '230220':
        wcs_instance = TwhWcs_LoopManualPacker(wcs_instance_id, deposit_queue)
        g_leaders[wcs_instance_id] = wcs_instance
        return wcs_instance
    

    else:
        Logger.Error("CreateWcsInstance()  Error")
        Logger.Print('wcs_instance_id', wcs_instance_id)
        return None # type: ignore