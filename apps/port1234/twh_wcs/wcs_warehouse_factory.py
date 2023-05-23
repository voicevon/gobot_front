from twh_wcs.von.wcs.pastor_base import PastorBase

# from twh_wcs.wcs_workers_factory import WcsWorkers
# from twh_wcs.wcs_component_factory import Components
from twh_wcs.von.wcs.workers.wcs_item_workers import WcsWorkers
from twh_wcs.von.wcs.components.wcs_item_components import Components
from twh_wcs.wcs_deck_factory import Wcs_DeckBase

from von.logger import Logger

class WarehouseItems:
    name = 'not named warehouse'
    pastors = dict[str, PastorBase]()
    workers_take = WcsWorkers()
    components_take = Components()
    decks = dict[str, dict[str,list[Wcs_DeckBase]]]()

g_warehouses = dict[str, WarehouseItems]()



# test area
# g_warehouses['a'] = WarehouseItems()
# g_warehouses['221109'].components_take.binary_outputs['picking'].Leds[3].GetState()
# x = g_warehouses['d'].workers_take.loop_porters[2]


class WarehouseFactory:

    @classmethod
    def EachWorker_SpinOnce(cls):
        for warehouse in g_warehouses.values():
            for porter in warehouse.workers_take.loop_porters:
                porter.SpinOnce()

    @classmethod
    def Create_Warehouse(cls, warehouse_id:str):
        if warehouse_id == '221109':
            new_warehouse = WarehouseItems()
            new_warehouse.name = '某某义齿加工厂'
            g_warehouses[warehouse_id] = new_warehouse
            return new_warehouse
        
        elif warehouse_id == '230220':
            new_warehouse = WarehouseItems()
            new_warehouse.name = '山东雅乐福义齿加工厂'
            g_warehouses[warehouse_id] = new_warehouse
            return new_warehouse
        
        else:
            Logger.Error("WarehouseFactory::Create_Warehouse() Unknown warehouse_id" )
            Logger.Print("warehouse_id",warehouse_id)

