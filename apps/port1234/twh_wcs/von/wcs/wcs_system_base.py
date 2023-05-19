

from twh_wcs.von.wcs.order import  Wcs_OrderBase, Wcs_OrderItemBase
# from twh_wcs.wcs_base.order_scheduler import Wcs_OrderSchedulerBase
from twh_wcs.von.wcs.porter.loop_porter import LoopPorter

import multiprocessing
from abc import ABC, abstractmethod
from von.mqtt.mqtt_agent import g_mqtt


class Wcs_SystemBase(ABC):

    def __init__(self, wcs_unit_id:str, deposit_queue:multiprocessing.Queue, order_scheduler: Wcs_OrderBase) -> None:
        self._orders_scheduler = order_scheduler

        self._wcs_unit_id = wcs_unit_id
        self._deposit_queue = deposit_queue
        self._wcs_state = 'idle'  
        # self._porters = list[Wcs_PorterBase]()
        # for p in self._porters:
        #     p.MoveTo(3,54)
        # __button_pick is a green button sit on packer.

        self.__showing_wcs_state = ''

    @abstractmethod
    def _deposit_queue_is_empty(self):
        pass

    @abstractmethod
    def _withdraw_queue_is_empty(self):
        pass

    def SpinOnce(self) ->str:
        '''
        return:  _wcs_state
        '''
        # Logger.Debug("TwhWcs_Unit::SpinOnce()")
        # Logger.Print("my twh_id", self._wcs_unit_id)
        if self._wcs_state == 'idle':
            if self._deposit_queue_is_empty():
                self._wcs_state = 'withdraw_order_item'
            else:
                self._wcs_state = 'deposit_begin'
        if self._wcs_state == 'deposite_begin':
            if self._deposit_queue_is_empty():
                self._wcs_state = 'idle'
        if self._wcs_state == 'withdraw_order_item':
            self._orders_scheduler.SpinOnce()
            if self._withdraw_queue_is_empty():
                self._wcs_state = 'idle'

        self._orders_scheduler.SpinOnce()
        if self.__showing_wcs_state != self._wcs_state:
            showing_wcs_state = self._wcs_state
            g_mqtt.publish('twh/' + self._wcs_unit_id + '/wcs_state',showing_wcs_state)
        return self._wcs_state

    def all_loop_porter_are_idle(self) -> bool:
        for porter in self._porters:
            if porter.GetState() != 'idle':
                return False
        return True

    # def Find_LoopPorter_ready(self) -> Wcs_PorterBase:
    #     for porter in self._porters:
    #         if porter.GetState() == 'ready':
    #             return porter
    #     return None # type: ignore
