from twh_wcs.von.wcs.workers.porter.loop_porter import LoopPorter
from twh_wcs.von.wcs.workers.pick_placer.pick_placer import Wsc_PickPlacerBase
from twh_wcs.von.wcs.workers.shipper.shipper import Wcs_ShipperBase
from twh_wcs.von.wcs.conveyor.tube_conveyor import TubeConveyor


class WcsWorkers:
    loop_porters = list[LoopPorter]()
    tube_conveyors = list[TubeConveyor]()
    pick_placers = list[Wsc_PickPlacerBase]()
    shippers = list[Wcs_ShipperBase]()