
	# House                Y+
	# 					 ^
	# 		 r0          |                                                                     \ beta  
	# 	  r1     d0      |                                                                    \  /
	# 	r2      d1       |                                                                     \/      
	#   r3       d3  d2    |                                                                   /  alpha
	#   -----------------(0,0)------Neck----------Head    --> X+                               --------------------------------->  Alpha,Beta (0,0)
	#   r4      d4  d5     |
	#    r5       d6       |
	# 	 r6     d7       |
	# 	  r7             |










# from gogame.chessboard_cell import ChessboardCell
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


class HouseSenceSite():

    def __init__(self) -> None:
        self.__NECK = (55.0, 0)
        self.__HEAD = (148.93, 0)
        self.__PARKING = (55.0, -2)

        self.__DOORS = [(-10.37,  59.07),
                (-21.39, 56.12),
                (-44.28, 40.67),
                (-57.42, 17.95),
                (-57.42, -17.95),
                (-44.28, -40.67),
                (-21.39, -56.12),
                (-10.37, -59.07),
                ]

        self.__ROOMS  = [(-78.79,  126.38),  #0
                (-98.18, 95.3),              #1
                (-111.9, 61.4),              #2
                (-119.51, 25.59),            #3
                (-119.51, -25.59),           #4
                (-111.9, -61.4),             #5
                (-98.18, -95.2),             #6
                (-78.79, -126.38)            #7
                ]
        self.x, self.y = self.__NECK


    def from_site_name(self, site_name):
        return self.__DOORS[1]

    def from_type_index(self,site_type:str, index=0 ):
        return self.__HEAD

    def to_gcode(self)->str:
        gcode = 'G1X' + str(self.x) + 'Y' + str(self.y)
        return gcode

class HumanLevelGobotHouse(HumanLevelRobotBase):

    def __init__(self, rabbit_client:RabbitClient) -> None:
        super().__init__(rabbit_client=rabbit_client)
        self.Home()
    
    def Home(self):
        self.rabbit_client.PublishToHouse('G28BI')
        self.rabbit_client.PublishToHouse('G28AI')
        self.rabbit_client.PublishToHouse('M996')
        
    def __PickupFrom(self, position_or_site:HouseSenceSite):
        gcode = position_or_site.to_gcode()
        self.rabbit_client.PublishToHouse(gcode=gcode)
    
    def PlaceTo(self, position_or_site:HouseSenceSite):
        x,y = self.SencePoint_ToXY(position_or_site)
        gcode ="G1X" + str(x) + 'Y' + str(y)
        self.rabbit_client.PublishToHouse(gcode=gcode)


    def Pickup_Place(self, from_where:HouseSenceSite, to_where:HouseSenceSite, auto_park=False):
        self.PickupFrom(from_where)
        self.PlaceTo(to_where)
        if auto_park:
            site = HouseSenceSite()
            site.from_site_name("parking")
            gcode = site.to_gcode()
            self.rabbit_client.PublishToHouse(gcode)
        self.rabbit_client.PublishToHouse('M996')

    def MoveTo(self, to_where:HouseSenceSite):
        gcode = to_where.to_gcode()
        # print('================================',gcode)
        self.rabbit_client.PublishToHouse(gcode)

    def PickupFrom(self, position_or_site):
        pass

    def demo(self):
        site = HouseSenceSite()
        for index in range(8):
            site.from_site_name('D' + str(index))
            self.MoveTo(site)
            site.from_site_name('R' + str(index))
            self.MoveTo(site)
        self.rabbit_client.PublishToHouse('M996')

