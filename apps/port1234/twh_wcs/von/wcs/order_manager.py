
from twh_wcs.von.wcs.order import Wcs_OrderBase, Wcs_OrderItemBase

from von.logger import Logger
from abc import ABC, abstractmethod

class Wcs_OrderMangerBase(ABC):

    def __init__(self, warehouse_id:str) -> None:
        ''' In WCS, An order's life time:
        * Created by: UI, or WMS
        * Main processes are:  porting, picking, packing, shipping.
        * Ended when shipped.
        Note:
        * An order item,  it might stored in different location, saying:  be served by different porter.
        '''

        self._warehouse_id = warehouse_id
        # self.order_id = order_id
        # self._all_order_items = list[Wcs_OrderItemBase]()
        # self.__state = 'idle'
        self._withdraw_orders = list[Wcs_OrderBase]() 

    def _find_this_order_by_id(self, order_id:str) -> Wcs_OrderBase:
        for order in self._withdraw_orders:
            if order.order_id == order_id:
                return order
        return None # type: ignore

    def GetWithdrawOrdersCount(self) -> int:
        return len(self._withdraw_orders)
    
    # def GetState(self) -> str:
    #     return self.__state

    # def IsFullFilled(self) -> bool:
    #     for t in self._all_order_items:
    #         if t.GetLocated() != 'packer':
    #             return False
    #     return True
    
    # def SpinOnce(self):
    #     '''
    #     return:
    #         -1   no released packer_cell
    #         0:11 packer_cell_id,  which has benn shipped out. should be released.
    #     '''
    #     # Logger.Debug('loop_tube wcs_system::  Twh_OrderHelper::SpinOnce()  my wcs_unit_id  = ' + self.wcs_unit_id)
    #     self._renew_orders_from_database()
        
    #     for order in self._withdraw_orders:
    #         is_shipped =  order.SpinOnce()
    #         if is_shipped:
    #             # Logger.Info(twh_factories[self.wcs_unit_id]['name'] +  ' -- WithdrawOrderManager:: SpinOnce().  Order is shipped')
    #             Logger.Info(self._warehouse_id +  ' -- WithdrawOrderManager:: SpinOnce().  Order is shipped')
    #             self._withdraw_orders.remove(order)
    #             return
            
    def SpinOnce(self):
        '''
        return:
            -1   no released packer_cell
            0:11 packer_cell_id,  which has benn shipped out. should be released.
        '''
        self._renew_orders_from_database()
        
        # Logger.Debug('aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa')
        for order in self._withdraw_orders:
            # Logger.Print('order---id ', order.order_id)
            is_shipped =  order.SpinOnce()
            if is_shipped:
                Logger.Info(self._warehouse_id +  ' -- WithdrawOrderManager:: SpinOnce().  Order is shipped')
                self._withdraw_orders.remove(order)
                return

    @abstractmethod
    def _renew_orders_from_database(self):
        pass



        