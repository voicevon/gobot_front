
from abc import ABC, abstractmethod

class Wsc_PickPlacerBase:
    def __init__(self) -> None:
        self._state = 'idle'

    def SpinOnce(self):
        pass

    def GetState(self) -> str:
        return self._state

    @abstractmethod
    def Start(self):
        pass

    @abstractmethod
    def _SpinOnce(self):
        pass

    
        
    

