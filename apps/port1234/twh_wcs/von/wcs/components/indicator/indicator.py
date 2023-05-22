from abc import ABC, abstractmethod

class Wcs_IndicatorBase(ABC):
    
    def __init__(self, index: int) -> None:
        self.index = index

    @abstractmethod
    def GetState(self):
        pass

    # @abstractmethod
    # def TurnOn(self):
    #     pass

    # @abstractmethod
    # def TurnOff(self):
    #     pass
