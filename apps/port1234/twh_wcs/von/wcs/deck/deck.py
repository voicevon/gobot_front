from twh_wcs.von.wcs.workers.worker_base import Wcs_WorkerBase

from von.logger import Logger
from abc import abstractmethod


class Wcs_DeckBase(Wcs_WorkerBase):

    def __init__(self, index:int) -> None:
        self._index = index
        self._state = 'idle'

    def GetState(self) -> str:
        return self._state
    
    def SetStateTo(self, new_state:str):
        if new_state in ['idle', 'loading', 'loaded']:
            self._state = new_state
        else:
            Logger.Error("Wcs_PackerBase:: SetStateTo()")
            Logger.Print("new_state", new_state)

    # @abstractmethod
    # def TurnOn_BeingAssigned_Led(self, index:int):
    #     '''
    #     Assigned to an order. although the packer is bland at the time.
    #     '''

    #     pass

    # @abstractmethod
    # def TurnOn_Placing_Led(self, index:int):
    #     '''
    #     Place order_item to here.
    #     '''
    #     pass

    # @abstractmethod
    # def TurnOn_Fullfilled_Led(self, index:int):
    #     '''
    #     Fullfilled, but still being occupied.
    #     '''
    #     pass

            