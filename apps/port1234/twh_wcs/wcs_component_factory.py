
from twh_wcs.von.wcs.components.button.button import Wcs_ButtonBase
from twh_wcs.von.wcs.components.indicator.indicator import Wcs_IndicatorBase
from twh_wcs.von.wcs.components.actuator.actuator import Wcs_3wayValve, Wcs_ActuatorBase

class Components:
    indicators = dict[str, list[Wcs_IndicatorBase]]()
    buttons = dict[str,list[Wcs_ButtonBase]]()
    actuators = dict[str,list[Wcs_ActuatorBase]]()


g_components = dict[str, Components]()

class ComponentFactory:
    def __init__(self) -> None:
        pass

    @classmethod
    def CreateLeds(cls, warehouse_id:str):
        leds = list[Wcs_IndicatorBase]()
        for led_index in range(7):
            new_led = Wcs_IndicatorBase(led_index)
            leds.append(new_led)
        g_components[warehouse_id].indicators['loop_porter_layer'] = leds


