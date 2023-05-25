

from twh_wcs.von.wcs.order_manager import Wcs_OrderMangerBase

import multiprocessing
from abc import ABC, abstractmethod
from von.mqtt.mqtt_agent import g_mqtt
from von.logger import Logger


class PastorBase(ABC):

    @abstractmethod
    def _start_deposit(self, new_deposit_request):
        pass
 
    @abstractmethod
    def _end_deposit(self):
        pass

    def __init__(self, warehouse_id:str, deposit_queue:multiprocessing.Queue, withdraw_order_manager: Wcs_OrderMangerBase) -> None:
        self._warehouse_id = warehouse_id
        self.__state = 'idle'  
        self.__withdraw_order_manager = withdraw_order_manager
        self.__deposit_queue = deposit_queue
        self.__showing_state = ''


    def SpinOnce(self) -> None:
        '''
        return:  __state
        '''
        # Logger.Debug("TwhWcs_Unit::SpinOnce()")
        # Logger.Print("my twh_id", self._wcs_unit_id)
        if self.__state == 'idle':
            if self.__deposit_queue.qsize() > 0:
                self._start_deposit(self.__deposit_queue.get())
                self.__state = 'deposit_begin'
            else:
                self.__state = 'withdraw_dispaching'
        if self.__state == 'deposite_begin':
            if self.__deposit_queue.qsize() == 0:
                self._end_deposit()
                self.__state = 'idle'
        if self.__state == 'withdraw_dispaching':
            self.__withdraw_order_manager.SpinOnce()    # Solution A
            if self.__withdraw_order_manager.GetWithdrawOrdersCount() == 0:
                self.__state = 'idle'
        # self.__withdraw_order_manager.SpinOnce()  # Solution B

        if self.__showing_state != self.__state:
            self.__showing_state = self.__state
            g_mqtt.publish('twh/' + self._warehouse_id + '/__state', self.__showing_state)


