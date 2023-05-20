from twh_wcs.warehouse_loop_manual_pack.twh_order import Twh_Order
from twh_wcs.warehouse_loop_manual_pack.twh_order_item import Twh_OrderItem
from twh_database.db_withdraw_order import DB_WithdrawOrder
from twh_database.bolt_nut import twh_factories

from twh_wcs.wcs_workers_factory import g_workers
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
                # Logger.Print("hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh", '')
                if the_order is None:
                    Logger.Print('Create new order from datas-base', '')
                    # this is a new order, put it into my management.
                    new_order = Twh_Order(self._warehouse_id, db_tooth['order_id'])
                    # self.AddOrderTask(new_order)
                    self._withdraw_orders.append(new_order)
                    the_order = new_order
                    if not printed_logger_title:
                        Logger.Debug('loop_manual warehosue:: WithdrawOrderManager::__renew_orders_from_database() First')
                        Logger.Print('Factory_name', self._warehouse_id)
                        printed_logger_title = True
                    Logger.Print('WithdrawOrderManager::__renew_orders_from_database()   new_order_task is added to manager. Order_id', new_order.order_id)
                the_order.SetStateTo(db_tooth['order_state'], write_to_db=False)

                # order_tooth = the_order.FindTooth_from_doc_id(db_tooth.doc_id)
                order_tooth = the_order.FindItem_from_doc_id(db_tooth.doc_id)
                if order_tooth is None:
                    # Logger.Info("loop manual:: renew_orders_from_database()::Create new tooth from database.......")
                    loop_porter = g_workers[self._warehouse_id].loop_porters[db_tooth['row']]
                    picker = g_workers[self._warehouse_id].pick_placers[0]
                    new_tooth = Twh_OrderItem(self._warehouse_id, db_tooth.doc_id, loop_porter, picker)
                    new_tooth.DentalLocation = db_tooth['location']
                    new_tooth.row = db_tooth['row']
                    new_tooth.col = db_tooth['col']
                    new_tooth.layer = db_tooth['layer']
                    the_order.AddItem(new_tooth)
                    order_tooth = new_tooth
                    if not printed_logger_title:
                        Logger.Debug('WithdrawOrderManager::__renew_orders_from_database()  Second')
                    Logger.Print('loop_manual::new_tooth is added to order_task. DentalLocation', new_tooth.DentalLocation)
                # order_tooth.TransferToLocated(db_tooth['located'], write_to_db=False)

            # if order_task.GetState() == 'shipped':
            #     DB_WithdrawOrder.delete_by_order_id(order_task.Order_id)
            #     self.__all_twh_orders.remove(order_task)



