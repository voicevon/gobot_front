
from abc import ABC, abstractmethod

class Wsc_PickPlacerBase(ABC):
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
    def SpinOnce(self):
        pass

    @abstractmethod
    def Start(self, pick_at, place_at):
        pass


    
        
    

