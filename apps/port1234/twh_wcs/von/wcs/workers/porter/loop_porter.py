# from twh_wcs.von.wcs.porter.porter import Wcs_PorterBase
from twh_wcs.von.wcs.workers.porter.porter import Wcs_PorterBase

class LoopPorter(Wcs_PorterBase):

    def __init__(self, wcs_unit_id: str, row_id: int, gcode_topic, state_topic) -> None:
        super().__init__(wcs_unit_id, row_id, gcode_topic, state_topic)

    