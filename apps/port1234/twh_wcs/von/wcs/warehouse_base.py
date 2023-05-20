

from twh_wcs.von.wcs.order import  Wcs_OrderBase, Wcs_OrderItemBase
from twh_wcs.von.wcs.order_manager import Wcs_OrderMangerBase

import multiprocessing
from abc import ABC, abstractmethod
from von.mqtt.mqtt_agent import g_mqtt
from von.logger import Logger


class WarehouseBase(ABC):

    def __init__(self, wcs_instance_id:str, deposit_queue:multiprocessing.Queue, withdraw_order_manager: Wcs_OrderMangerBase) -> None:
        self._warehouse_id = wcs_instance_id
        self._wcs_state = 'idle'  
        self.__withdraw_order_manager = withdraw_order_manager
        self.__deposit_queue = deposit_queue
        self.__showing_wcs_state = ''


    def SpinOnce(self) ->str:
        '''
        return:  _wcs_state
        '''
        # Logger.Debug("TwhWcs_Unit::SpinOnce()")
        # Logger.Print("my twh_id", self._wcs_unit_id)
        if self._wcs_state == 'idle':
            if self.__deposit_queue.qsize() > 0:
                self._wcs_state = 'deposit_begin'
            else:
                self._wcs_state = 'withdraw_order_item'
        if self._wcs_state == 'deposite_begin':
            if self.__deposit_queue.qsize() == 0:
                self._wcs_state = 'idle'
        if self._wcs_state == 'withdraw_order_item':
            # self.__withdraw_order_manager.SpinOnce()
            if self.__withdraw_order_manager.GetWithdrawOrdersCount() == 0:
                self._wcs_state = 'idle'

        self.__withdraw_order_manager.SpinOnce()
        # Logger.Print('__showing_wcs_state', self.__showing_wcs_state)
        # Logger.Print('wcs_state', self._wcs_state)
        if self.__showing_wcs_state != self._wcs_state:
            self.__showing_wcs_state = self._wcs_state
            g_mqtt.publish('twh/' + self._warehouse_id + '/wcs_state', self.__showing_wcs_state)
        return self._wcs_state

    # def all_loop_porter_are_idle(self) -> bool:
    #     for porter in self._porters:
    #         if porter.GetState() != 'idle':
    #             return False
    #     return True

 
