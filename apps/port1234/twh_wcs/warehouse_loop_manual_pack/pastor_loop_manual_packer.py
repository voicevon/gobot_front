from twh_wcs.von.wcs.pastor_base import PastorBase
from twh_wcs.warehouse_loop_manual_pack.twh_order_manager import Twh_OrderManager

from twh_wcs.wcs_warehouse_factory import g_warehouses
from twh_database.bolt_nut import twh_factories

from von.logger import Logger
import multiprocessing


class Pastor_LoopManualPacker(PastorBase):

    def __init__(self, wcs_instance_id:str, deposit_queue:multiprocessing.Queue) -> None:
        
        self.__order_manager = Twh_OrderManager(wcs_instance_id)

        super().__init__(wcs_instance_id, deposit_queue, self.__order_manager)

    def _start_deposit(self, new_deposit_request):
        Logger.Info(twh_factories[self._warehouse_id]['name'] + " -- Twh_WarehouseControlSystem::Do_deposit() ")
        Logger.Print("new_deposit_request", new_deposit_request)
        # the loop-porter will move to col-position
        row_id = int(new_deposit_request['row'])
        col_id = int(new_deposit_request['col'])
        layer_id = int(new_deposit_request['layer'])
        # Logger.Print("row_id", row_id)
        # Logger.Print("porters count", len(self.__porters))
        # porter = self.__porters[row_id]
        # porter = g_workers[self._warehouse_id].loop_porters[row_id]
        porter = g_warehouses[self._warehouse_id].workers_take.loop_porters[row_id]
        self.__depositing_porter = porter
        Logger.Print('layer_id', layer_id)
        porter._MoveTo(col_id, layer_id)
        porter._show_layer_led()
        # Logger.Print("Twh_WarehouseControlSystem::Do_deposit()    point", 99)

    def _end_deposit(self):
        self.__depositing_porter._turn_off_leds()
 
