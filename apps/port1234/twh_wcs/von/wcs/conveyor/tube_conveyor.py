from twh_wcs.wcs_base.porter_conveyor import Wcs_PorterBase


class TubePorter(Wcs_PorterBase):
    def __init__(self, wcs_unit_id: str, row_id: int, gcode_topic, state_topic) -> None:
        super().__init__(wcs_unit_id, row_id, gcode_topic, state_topic)