
from twh_wcs.von.wcs.components.indicator.indicator import Wcs_IndicatorBase
from twh_wcs.von.wcs.components.actuator.actuator import Wcs_3wayValve, Wcs_ActuatorBase
from twh_wcs.von.wcs.components.binary_output.grouped_binary_output import BinaryOutputGroup

from twh_wcs.von.wcs.components.binary_input.binary_input import Wcs_ButtonBase


class Components:
    buttons = dict[str,Wcs_ButtonBase]()
    binary_outputs = dict[str, BinaryOutputGroup]()
    actuators = dict[str,list[Wcs_ActuatorBase]]()