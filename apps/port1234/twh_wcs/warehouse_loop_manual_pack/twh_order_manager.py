from twh_wcs.warehouse_loop_manual_pack.twh_order import Twh_Order
from twh_database.db_withdraw_order import DB_WithdrawOrder

# from twh_wcs.wcs_workers_factory import WorkersFactory
from twh_wcs.wcs_warehouse_factory import g_warehouses
from twh_wcs.von.wcs.order_manager import Wcs_OrderMangerBase
from von.logger import Logger


class Twh_OrderManager(Wcs_OrderMangerBase):

    def __init__(self, wcs_instance_id:str) -> None:
        super().__init__(wcs_instance_id)
          
    def _renew_orders_from_database(self):
        '''
        1. renew all orders from database
        2. renew teeth state inside order (the state is from database)
        3. turorial note: https://tinydb.readthedocs.io/en/latest/usage.html
        The TinyDB query cache doesn't check if the underlying storage that the database uses has been modified by an external process. 
        In this case the query cache may return outdated results. 
        To clear the cache and read data from the storage again you can use db.clear_cache().
                
        '''
        # Logger.Debug('loop-manual warehouse:: Twh_OrderManager:: renew_order_state_from_database()')
        printed_logger_title = False
        DB_WithdrawOrder.table_withdraw_order.clear_cache()
        db_order_teeth =  DB_WithdrawOrder.table_withdraw_order.all()
        for db_tooth in db_order_teeth:
            # Logger.Print("db_tooth['order_id']",db_tooth['order_id'])
            the_order = self._find_this_order_by_id(db_tooth['order_id'])
            if db_tooth['twh_id'] == self._warehouse_id:   # TODO:  move into db_order_teeth  searching.
                if the_order is None:
                    Logger.Print('Create new order from database', '')
                    # this is a new order, put it into my management.
                    new_order = Twh_Order(self._warehouse_id, db_tooth['order_id'])
                    self._withdraw_orders.append(new_order)
                    the_order = new_order
                    if not printed_logger_title:
                        Logger.Debug('loop_manual warehosue:: WithdrawOrderManager::__renew_orders_from_database() First')
                        Logger.Print('Warehouse name', g_warehouses[self._warehouse_id].name)
                        printed_logger_title = True
                    Logger.Print('WithdrawOrderManager::__renew_orders_from_database()   new_order is added to manager. Order_id', new_order._order_id)
                