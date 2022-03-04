from abc import ABC, abstractmethod

from rabbitmq_all_in_one import RabbitClient


class HumanLevelRobotBase(ABC):
    def __init__(self, rabbit_client:RabbitClient) -> None:
        self.rabbit_client = rabbit_client

    @abstractmethod
    def PickupFrom(self, position_or_site):
        pass

    @abstractmethod
    def PlaceTo(self, position_or_site):
        pass

    @abstractmethod
    def Pickup_Place(self, from_where, to_where, auto_park=False):
        pass