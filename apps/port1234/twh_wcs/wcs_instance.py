from twh_wcs.twhwcs_common.twh_robot_loop_porter import Twh_LoopPorter


from twh_wcs.von.wcs.wcs_system_base import Wcs_SystemBase
# from twhwcs_loop_tube_system.twhwcs_loop_tube import Twh_LoopTubeSystem

from twh_wcs.von.wcs.conveyor.tube_conveyor import TubeConveyor
from twh_wcs.von.wcs.porter.loop_porter import LoopPorter
from twh_wcs.von.wcs.packer.packer import Wcs_PackerBase
from twh_wcs.von.wcs.shipper.shipper import Wcs_ShipperBase

# from twh_wcs.twhwcs_loop_tube_system.my_subsystem_hardware import g_subsystem_porters, g_subsystem_tube_coneyor

# g_subsystem_porters = list[Twh_LoopPorter]()
# g_subsystem_tube_coneyor = TubeConveyor('221109', 1,'gcode_topic', 'state_topc')

from von.logger import Logger


class WcsInstance:
    software: Wcs_SystemBase
    loop_porters = list[LoopPorter]()
    tube_conveyors = list[TubeConveyor]()
    packers = list[Wcs_PackerBase]()
    shipper = list[Wcs_ShipperBase]()


g_wcss = dict[str, WcsInstance]()

def CreateWcsInstance(wcs_unit_id:str) -> WcsInstance:
    instance = WcsInstance()

    if wcs_unit_id == '221109':
        g_wcss[wcs_unit_id] = instance
        for i in range(1):
            new_porter = Twh_LoopPorter(wcs_unit_id, i)
            instance.loop_porters.append(new_porter)
        for i in range(1):
            new_tube_conveyor = TubeConveyor(wcs_unit_id, 0 ,'','')
            instance.tube_conveyors.append(new_tube_conveyor)
        return instance
    
    elif wcs_unit_id == '230220':
        g_wcss[wcs_unit_id] = instance
        return instance
    

    else:
        Logger.Error("CreateWcsInstance()  Error")
        Logger.Print('wcs_unit_id', wcs_unit_id)
        return None # type: ignore