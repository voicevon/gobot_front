from twh_wcs.twhwcs_loop_tube_system.twh_order import Twh_Order, Twh_OrderItem
from twh_database.db_withdraw_order import DB_WithdrawOrder
from twh_database.bolt_nut import twh_factories


from twh_wcs.von.wcs.order_manager import Wcs_OrderMangerBase
from von.logger import Logger


class Twh_OrderManager(Wcs_OrderMangerBase):

    # def __init__(self, twh_id:str, packer:TwhRobot_Packer, shipper:TwhRobot_Shipper) -> None:
    def __init__(self, twh_wcs_unit_id:str) -> None:
        ''' In WCS, An order's life time:
        * Created by: UI, or WMS
        * Main processes are:  porting, picking, packing, shipping.
        * Ended when shipped.
        Note:
        * An order item,  it might stored in different location, saying:  be served by different porter.
        '''
        super().__init__(twh_wcs_unit_id)
        ''' 
        life time: created by UI, or WMS,  ended when the order is shipped.
        Q:  Should this be in super?
        A:  Looks like should be.  Need to consider more.
        '''

    def FindTooth_is_in_porter_from_all_orders(self, porter_id:int) -> tuple[Twh_OrderItem, Twh_Order]:
        '''
        * porter_id is equal to tooth.row.
        * constraint:  tooth must be located in porter
        '''
        # Logger.Print('OrderTaskManager:: FindTooth_is_in_porter()   len(all_withdraw_order)  ', len(self.__all_twh_orders))
        for order in self._withdraw_orders:
            tooth = order.FindTooth_is_in_porter(porter_id)
            if tooth is not None:
                # Logger.Print('found tooth in the loop-porter,  tooth.col', tooth.col)
                return tooth, order
        return None,None # type: ignore
        
    def _renew_orders_from_database(self):
        '''
        1. renew all orders from database
        2. renew teeth state inside order (the state is from database)
        3. turorial note: https://tinydb.readthedocs.io/en/latest/usage.html
        The TinyDB query cache doesn't check if the underlying storage that the database uses has been modified by an external process. 
        In this case the query cache may return outdated results. 
        To clear the cache and read data from the storage again you can use db.clear_cache().
                
        '''
        # Logger.Debug('Twh_WarehouseControlSystem:: renew_order_state_from_database()')
        printed_logger_title = False
        DB_WithdrawOrder.table_withdraw_order.clear_cache()
        db_order_teeth =  DB_WithdrawOrder.table_withdraw_order.all()
        for db_tooth in db_order_teeth:
            the_order = self.FindWithdrawOrder(db_tooth['order_id'])
            if db_tooth['twh_id'] == self.wcs_unit_id:   # TODO:  move into db_order_teeth  searching.
                if the_order is None:
                    new_order = Twh_Order(db_tooth['order_id'])
                    # self.AddOrderTask(new_order)
                    self._withdraw_orders.append(new_order)
                    the_order = new_order
                    if not printed_logger_title:
                        Logger.Debug('WithdrawOrderManager::__renew_orders_from_database() First')
                        Logger.Print('Factory_name', self.wcs_unit_id)
                        printed_logger_title = True
                    Logger.Print('WithdrawOrderManager::__renew_orders_from_database()   new_order_task is added to manager. Order_id', new_order.order_id)
                the_order.SetStateTo(db_tooth['order_state'], write_to_db=False)

                order_tooth = the_order.FindTooth_from_doc_id(db_tooth.doc_id)
                if order_tooth is None:
                    new_tooth = Twh_OrderItem(db_tooth.doc_id)
                    new_tooth.DentalLocation = db_tooth['location']
                    new_tooth.row = db_tooth['row']
                    new_tooth.col = db_tooth['col']
                    new_tooth.layer = db_tooth['layer']
                    the_order.AddTooth(new_tooth)
                    order_tooth = new_tooth
                    if not printed_logger_title:
                        Logger.Debug('WithdrawOrderManager::__renew_orders_from_database()  Second')
                    Logger.Print('new_tooth is added to order_task. DentalLocation', new_tooth.DentalLocation)
                order_tooth.TransferToLocated(db_tooth['located'], write_to_db=False)

            # if order_task.GetState() == 'shipped':
            #     DB_WithdrawOrder.delete_by_order_id(order_task.Order_id)
            #     self.__all_twh_orders.remove(order_task)



