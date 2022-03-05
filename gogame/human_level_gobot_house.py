
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









import sys
sys.path.append('C:\\gitlab\\gobot_front')  # For runing in VsCode on Windows-10 
from gogame.human_level_robot_base import HumanLevelRobotBase
from rabbitmq_all_in_one import RabbitClient
import enum


class HumanLevelHouse_EEF_ACTIONS(enum.Enum):
    LOAD = 3,
    SLEEP = 5,


class HouseMapDiction():

    def __init__(self) -> None:
        self.NECK = (55.0, 0)
        self.HEAD = (148.93, 0)
        self.PARKING = (55.0, -2)

        self.DOORS = [(-10.37,  59.07),
                (-21.39, 56.12),
                (-44.28, 40.67),
                (-57.42, 17.95),
                (-57.42, -17.95),
                (-44.28, -40.67),
                (-21.39, -56.12),
                (-10.37, -59.07),
                ]

        self.ROOMS  = [(-78.79,  126.38),  #0
                (-98.18, 95.3),              #1
                (-111.9, 61.4),              #2
                (-119.51, 25.59),            #3
                (-119.51, -25.59),           #4
                (-111.9, -61.4),             #5
                (-98.18, -95.2),             #6
                (-78.79, -126.38)            #7
                ]


class HumanLevelGobotHouse(HumanLevelRobotBase):

    def __init__(self, rabbit_client:RabbitClient) -> None:
        super().__init__(rabbit_client=rabbit_client)
        self.Home()
    
    def Home(self):
        self.rabbit_client.PublishToHouse('G28BI')
        self.rabbit_client.PublishToHouse('G28AI')
        self.rabbit_client.PublishToHouse('M996')
        
    def Pickup_Place(self, from_where:HouseMapDiction, to_where:HouseMapDiction, auto_park=False):
        self.PickupFrom(from_where)
        self.PlaceTo(to_where)
        if auto_park:
            site = HouseMapDiction()
            x,y = site.PARKING
            self.MoveTo(x,y)
        self.rabbit_client.PublishToHouse('M996')

    def MoveTo(self, x:float, y:float):
        gcode = 'G1X' + str(x) + 'Y' + str(y)
        self.rabbit_client.PublishToHouse(gcode)

    def PickupFrom(self, position_or_site):
        pass

    def PlaceTo(self, position_or_site):
        return super().PlaceTo(position_or_site)

    def EefAction(self, eef: HumanLevelHouse_EEF_ACTIONS):
        if eef==HumanLevelHouse_EEF_ACTIONS.LOAD:
            self.rabbit_client.PublishToHouse('M123P1S128')
        elif eef==HumanLevelHouse_EEF_ACTIONS.SLEEP:
            self.rabbit_client.PublishToHouse('M123P1S0')

    def Test_Eef(self):
        self.rabbit_client.PublishToHouse('M123P1S128')
        self.rabbit_client.PublishToHouse('G4S1')
        self.rabbit_client.PublishToHouse('M123P1S0')
        self.rabbit_client.PublishToHouse('G4S5')
        self.rabbit_client.PublishToHouse('M996')
        
        # self.rabbit_client.PublishToArm('M123P1S128')
        # self.rabbit_client.PublishToArm('G4S1')
        # self.rabbit_client.PublishToArm('M123P1S0')
        # self.rabbit_client.PublishToArm('G4S5')
        # self.rabbit_client.PublishToArm('M996')

    def demo(self):
        site = HouseMapDiction()
        x,y = site.DOORS[0]
        self.MoveTo(x,y)
        for index in range(8):
            x,y = site.ROOMS[index]
            self.MoveTo(x,y)
            self.EefAction(HumanLevelHouse_EEF_ACTIONS.LOAD)
            x,y = site.DOORS[index]
            self.MoveTo(x,y)
            self.EefAction(HumanLevelHouse_EEF_ACTIONS.SLEEP)
        x,y = site.DOORS[7]
        self.MoveTo(x,y)
        # Park Arms at a point, nearby homed position
        self.rabbit_client.PublishToHouse('G1B120F2800')
        self.rabbit_client.PublishToHouse('G1A-1F2800')
        self.rabbit_client.PublishToHouse('M996')




if __name__ == '__main__':
    from rabbitmq_all_in_one import RabbitMqClient_Helper
    helper = RabbitMqClient_Helper()
    client = helper.MakeClient()
    house = HumanLevelGobotHouse(client)
    for i in range(80):
        house.Test_Eef()

    while True:
        helper.SpinOnce()