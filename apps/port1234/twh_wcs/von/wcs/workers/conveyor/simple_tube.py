from twh_wcs.von.wcs.workers.conveyor.tube_conveyor import TubeConveyorBase
from twh_wcs.von.wcs.components.binary_output.grouped_binary_output import BinaryOutputGroup
from twh_wcs.wcs_warehouse_factory import g_warehouses


class SimpleTubeConveyor(TubeConveyorBase):

    def __init__(self, warehouse_id:str) -> None:
        super().__init__()
        self.__inlet_valves = g_warehouses[warehouse_id].components_take.binary_outputs['inlet_valves']
        self.__outlet_valves = g_warehouses[warehouse_id].components_take.binary_outputs['outlet_valves']
        
    def Start(self, inlet_index:int, outlet_index:int):
        self.__inlet_valves.SetState_for_All(is_turn_on=False)
        self.__inlet_valves.SetElementState(inlet_index, is_turn_on=True)
        self.__outlet_valves.SetState_for_All(is_turn_on=False)
        self.__outlet_valves.SetElementState(outlet_index, is_turn_on=True)


