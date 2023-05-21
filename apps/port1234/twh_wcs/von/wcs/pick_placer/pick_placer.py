
from twh_wcs.von.wcs.worker_base import Wcs_WorkerBase
from abc import abstractmethod

class Wsc_PickPlacerBase(Wcs_WorkerBase):
    def __init__(self) -> None:
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
    def Start(self, pick_at, place_at):
        pass


    
        
    

