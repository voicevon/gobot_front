
from abc import ABC, abstractmethod

class Wsc_PickPlacerBase(ABC):
    def __init__(self) -> None:
        self._state = 'idle'

    def GetState(self) -> str:
        return self._state

    @abstractmethod
    def SpinOnce(self):
        pass

    @abstractmethod
    def Start(self):
        pass


    
        
    

