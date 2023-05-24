
from twh_wcs.von.wcs.components.binary_input.binary_input import Wcs_ButtonBase
from twh_wcs.von.wcs.components.binary_output.grouped_binary_output import BinaryOutputGroup

from twh_wcs.von.wcs.components.wcs_item_components import Components
from twh_wcs.wcs_warehouse_factory import g_warehouses

from von.logger import Logger




# g_components = dict[str, Components]()
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
    def CreateComponents(cls, warehouse_id):
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

            fullfilled_leds = BinaryOutputGroup('wh221109/fullfilled_leds', 12)
            new_components.binary_outputs['fullfilled_leds'] = fullfilled_leds

            picking_leds = BinaryOutputGroup('wh221109/shiping_leds', 12)
            new_components.binary_outputs['shiping_leds'] = picking_leds

            placed_button = Wcs_ButtonBase("wh221109/placed_button",)
            new_components.buttons['placed'] = placed_button

            shipped_button = Wcs_ButtonBase("wh221109/shipped_button")
            new_components.buttons['shipped'] = shipped_button

            # g_components[warehouse_id] = new_components
            g_warehouses[warehouse_id].components_take = new_components
            return new_components
        
        elif warehouse_id == '230220':
            mqtt_topic = 'wh' + warehouse_id + 'tube_inlets'
            inlets = BinaryOutputGroup(mqtt_topic, 1)
            new_components.binary_outputs['inlet_valves'] = inlets

            mqtt_topic = 'wh' + warehouse_id + 'tube_outlets'
            outlets = BinaryOutputGroup(mqtt_topic, 5)
            new_components.binary_outputs['outlet_valves'] = outlets
            
            g_warehouses[warehouse_id].components_take = new_components
            return new_components

        else:
            Logger.Error("ComponentFactory::CreateCompnents()   Unknown warehouse_id")
            Logger.Print("warehouse_id", warehouse_id)
        return cls.CreateBlankComponents()

