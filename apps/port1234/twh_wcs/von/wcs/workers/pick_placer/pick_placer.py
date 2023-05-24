
from twh_wcs.von.wcs.workers.worker_base import Wcs_WorkerBase
from von.logger import Logger
from abc import abstractmethod

class Wsc_PickPlacerBase(Wcs_WorkerBase):
    def __init__(self, warehouse_id:str) -> None:
        super().__init__(warehouse_id)
        self._state = 'idle'
        '''
        All states:  on point view of PickPlacerBase
        * 'idle'
        * 'picking_placing'
        * 'picked_placed'
        '''

    def GetState(self) -> str:
        return self._state

    @abstractmethod
    def ResetStatemachine(self):
        pass

    @abstractmethod
    def Start(self, pick_at, place_at):
        pass

    # @abstractmethod
    def _print_out(self):
        super()._print_out()
        Logger.Print("_state", self._state)

    
        
    

