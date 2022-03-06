
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


class HumanLevel_GarmentAgv(HumanLevelRobotBase):

    def __init__(self, rabbit_client:RabbitClient) -> None:
        super().__init__(rabbit_client=rabbit_client)
        self.Home()
    
    def Home(self):
        self.rabbit_client.PublishToHouse('G28BI')
        self.rabbit_client.PublishToHouse('G28AI')
        self.rabbit_client.PublishToHouse('M996')
        
    def Pickup_Place(self, from_where, to_where, auto_park=False):
        self.PickupFrom(from_where)
        self.PlaceTo(to_where)
        if auto_park:
            site = (2.0, 3,0)
            x,y = site.PARKING
            self.MoveTo(x,y)
        self.rabbit_client.PublishToHouse('M996')

    def MoveTo(self, height:float, angle:float):
        gcode = 'G1Z' + str(height) + 'R' + str(angle)
        self.rabbit_client.PublishToHouse(gcode)

    def DisableMotor(self):
        self.rabbit_client.PublishToHouse('M84')

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
        self.rabbit_client.PublishToHouse('M123P1S3')   #load
        self.rabbit_client.PublishToHouse('G4S5')
        self.rabbit_client.PublishToHouse('M123P1S5')   #sleep
        self.rabbit_client.PublishToHouse('G4S5')
        self.rabbit_client.PublishToHouse('M996')
        
        # self.rabbit_client.PublishToArm('M123P1S128')
        # self.rabbit_client.PublishToArm('G4S1')
        # self.rabbit_client.PublishToArm('M123P1S0')
        # self.rabbit_client.PublishToArm('G4S5')
        # self.rabbit_client.PublishToArm('M996')

    def demo(self):
        site = (1,2)
        x,y = site
        self.MoveTo(x,y)

        # Park Arms at a point, nearby homed position
        self.rabbit_client.PublishToHouse('G1B120F2800')
        self.rabbit_client.PublishToHouse('G1A-1F2800')
        self.rabbit_client.PublishToHouse('M996')




if __name__ == '__main__':
    from rabbitmq_all_in_one import RabbitMqClient_Helper
    helper = RabbitMqClient_Helper()
    client = helper.MakeClient()
    house = HumanLevel_GarmentAgv(client)
    # for i in range(30):
    #     house.Test_Eef()
    house.MoveTo(350, 60)
    house.DisableMotor()

    while True:
        helper.SpinOnce()