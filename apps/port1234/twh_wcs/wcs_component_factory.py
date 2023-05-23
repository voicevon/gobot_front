
from twh_wcs.von.wcs.components.binary_input.binary_input import Wcs_ButtonBase
from twh_wcs.von.wcs.components.indicator.indicator import Wcs_IndicatorBase
from twh_wcs.von.wcs.components.binary_output.grouped_binary_output import BinaryOutputGroup
from twh_wcs.von.wcs.components.actuator.actuator import Wcs_3wayValve, Wcs_ActuatorBase

from von.logger import Logger

class Components:
    # indicators = dict[str, list[Wcs_IndicatorBase]]()
    buttons = dict[str,Wcs_ButtonBase]()
    binary_outputs = dict[str, BinaryOutputGroup]()
    actuators = dict[str,list[Wcs_ActuatorBase]]()

g_components = dict[str, Components]()
'''
Explain 
```
   g_components = dict[str, Components]()
                        |         
                        |--- warehouse_id
```'''

class ComponentFactory:
    def __init__(self) -> None:
        pass

    # @classmethod
    # def CreateLeds(cls, warehouse_id:str):
    #     leds = list[Wcs_IndicatorBase]()
    #     for led_index in range(7):
    #         new_led = Wcs_IndicatorBase(led_index)
    #         leds.append(new_led)
    #     g_components[warehouse_id].indicators['loop_porter_layer'] = leds

    @classmethod
    def CreateBlankComponents(cls) -> Components:
        return Components()
    
    @classmethod
    def CreateCompnents(cls, warehouse_id):
        new_components = Components()

        if warehouse_id == '221109':
            picking_leds = BinaryOutputGroup('wh221109/porter0/leds', 7)
            new_components.binary_outputs['porter0_leds'] = picking_leds

            picking_leds = BinaryOutputGroup('wh221109/porter1/leds', 7)
            new_components.binary_outputs['porter1_leds'] = picking_leds

            picking_leds = BinaryOutputGroup('wh221109/porter2/leds', 7)
            new_components.binary_outputs['porter2_leds'] = picking_leds

            picking_leds = BinaryOutputGroup('wh221109/porter3/leds', 7)
            new_components.binary_outputs['porter3_leds'] = picking_leds

            picking_leds = BinaryOutputGroup('wh221109/placing_leds', 12)
            new_components.binary_outputs['placing_leds'] = picking_leds

            picking_leds = BinaryOutputGroup('wh221109/shiping_leds', 12)
            new_components.binary_outputs['shiping_leds'] = picking_leds

            placed_button = Wcs_ButtonBase("wh221109/placed_button",)
            new_components.buttons['placed'] = placed_button

            shipped_button = Wcs_ButtonBase("wh221109/shipped_button")
            new_components.buttons['shipped'] = shipped_button

            g_components[warehouse_id] = new_components
            return new_components
        else:
            Logger.Error("ComponentFactory::CreateCompnents()   Unknown warehouse_id")
            Logger.Print("warehouse_id", warehouse_id)
        return cls.CreateBlankComponents()

