from abc import ABC, abstractmethod



# TODO:  remove this class
class HumanLevelRobotBase(ABC):
    def __init__(self) -> None:
        pass
        #TODO:  remove this line

    # @abstractmethod
    # def PickupFrom(self, position_or_site):
    #     pass

    # @abstractmethod
    # def PlaceTo(self, position_or_site):
    #     pass

    @abstractmethod
    def Pickup_Place(self, from_where, to_where, auto_park=False):
        pass