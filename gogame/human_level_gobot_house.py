from gogame.human_level_robot_base import HumanLevelRobotBase
from rabbitmq_all_in_one import RabbitClient
import enum


class HumanLeve_HouseEEF_ACTIONS(enum.Enum):
    SLEEP = 0
    LOAD = 1
    UNLOAD = 2
    MOVE_Z_TOP = 3
    MOVE_Z_MIDDLE = 4
    MOVE_Z_BOTTOM = 5

class HumanLevelGobotHouse(HumanLevelRobotBase):

    def __init__(self, rabbit_client:RabbitClient) -> None:
        super().__init__(rabbit_client=rabbit_client)

    def SencePoint_ToXY(self, position_or_site):
        return 1,5
    
    def Home(self):
        self.rabbit_client.PublishToHouse('G28BI')
        self.rabbit_client.PublishToHouse('G28AI')
        self.rabbit_client.PublishToHouse('M996')
        
    def PickupFrom(self, position_or_site):
        x,y = self.SencePoint_ToXY(position_or_site)
        gcode ="G1X" + str(x) + 'Y' + str(y)
        self.rabbit_client.PublishToHouse(gcode=gcode)
        self.rabbit_client.PublishToHouse('M996')
    
    def PlaceTo(self, position_or_site):
        x,y = self.SencePoint_ToXY(position_or_site)
        gcode ="G1X" + str(x) + 'Y' + str(y)
        self.rabbit_client.PublishToHouse(gcode=gcode)

    def move_to(self, position_name):
        x,y = self.SencePoint_ToXY(position_name)
        gcode ="G1X" + str(x) + 'Y' + str(y)
        self.rabbit_client.PublishToHouse(gcode=gcode)

    def pickup_stone_from_cell(self, position_name):
        pass


    def Pickup_Place(self, from_where, to_where,auto_park=False):
        self.PickupFrom(from_where)
        self.PlaceTo(to_where)
        if auto_park:
            self.move_to('park_lot')
        self.rabbit_client.PublishToHouse('M996')
