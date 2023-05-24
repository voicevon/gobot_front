from twh_wcs.von.wcs.workers.pick_placer.pick_placer import Wsc_PickPlacerBase
from twh_wcs.wcs_component_factory import Wcs_ButtonBase
from von.logger import Logger
from twh_wcs.wcs_warehouse_factory import g_warehouses

class Manual_PickPlacer(Wsc_PickPlacerBase):
    def __init__(self, warehouse_id:str) -> None:
        super().__init__(warehouse_id)
        self.__button_placed = g_warehouses[self.warehouse_id].components_take.buttons['placed']

    def Start(self):
        self.__button_placed.Reset()
        self._state = 'picking_placing'

    def SpinOnce(self):
        if self.__button_placed.IsPressed():
            self._state =  'picked_placed'

    def ResetStatemachine(self):
        if self._state in ['picked_placed', 'idle']:
            self._state = 'idle'
        else:
            Logger.Error("Manual_PickPlacer::ResetStatemachine()  Error")
            Logger.Print(self.warehouse_id +"  " + g_warehouses[self.warehouse_id].name, self._state)
        

    def PrintOut(self, title):
        Logger.Info(title)
        super()._print_out()
