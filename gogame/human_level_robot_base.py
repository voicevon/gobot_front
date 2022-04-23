from abc import ABC, abstractmethod

# from rabbitmq_app_examle_uselss import RabbitClient
from Pylib.rabbit_mq_basic import g_amq


class HumanLevelRobotBase(ABC):
    def __init__(self) -> None:
        #TODO:  remove this line
        self.rabbit_client = g_amq

    # @abstractmethod
    # def PickupFrom(self, position_or_site):
    #     pass

    # @abstractmethod
    # def PlaceTo(self, position_or_site):
    #     pass

    @abstractmethod
    def Pickup_Place(self, from_where, to_where, auto_park=False):
        pass