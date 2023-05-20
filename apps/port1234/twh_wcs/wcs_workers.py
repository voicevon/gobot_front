from twh_wcs.twhwcs_common.twh_robot_loop_porter import Twh_LoopPorter
from twh_wcs.von.wcs.pick_placer.manual_pick_placer import Manual_PickPlacer
from twh_wcs.von.wcs.shipper.manual_shipper import Manual_Shipper


from twh_wcs.von.wcs.conveyor.tube_conveyor import TubeConveyor
from twh_wcs.von.wcs.porter.loop_porter import LoopPorter
from twh_wcs.von.wcs.pick_placer.pick_placer import Wsc_PickPlacerBase
from twh_wcs.von.wcs.packer.packer import Wcs_PackerBase
from twh_wcs.von.wcs.shipper.shipper import Wcs_ShipperBase

from von.logger import Logger

class WcsWorkers:
    # software: Wcs_InstanceBase
    loop_porters = list[LoopPorter]()
    tube_conveyors = list[TubeConveyor]()
    pick_placers = list[Wsc_PickPlacerBase]()
    packers = list[Wcs_PackerBase]()
    shipper = list[Wcs_ShipperBase]()

g_workers = dict[str, WcsWorkers]()

def Create_WcsWorkers(wcs_instance_id:str) -> WcsWorkers:
    wcs_workers = WcsWorkers()

    if wcs_instance_id == '221109':
        g_workers[wcs_instance_id] = wcs_workers
        for i in range(4):
            new_porter = Twh_LoopPorter(wcs_instance_id, i)
            wcs_workers.loop_porters.append(new_porter)
        
        new_picker = Manual_PickPlacer("twh/" + wcs_instance_id + 'picker')
        wcs_workers.pick_placers.append(new_picker)
        
        new_shipper = Manual_Shipper("twh/" + wcs_instance_id + 'shipper/button')
        wcs_workers.shipper.append(new_shipper)
        return wcs_workers

    elif wcs_instance_id == '230220':
        g_workers[wcs_instance_id] = wcs_workers
        for i in range(1):
            new_porter = Twh_LoopPorter(wcs_instance_id, i)
            wcs_workers.loop_porters.append(new_porter)
        for i in range(1):
            new_tube_conveyor = TubeConveyor(wcs_instance_id, 0 ,'','')
            wcs_workers.tube_conveyors.append(new_tube_conveyor)
        return wcs_workers
    

    else:
        Logger.Error("CreateWcsInstance()  Error")
        Logger.Print('wcs_instance_id', wcs_instance_id)
        return None # type: ignore