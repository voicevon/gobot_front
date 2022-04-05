

import sys
sys.path.append('C:\\gitlab\\gobot_front')  # For runing in VsCode on Windows-10 
from gogame.human_level_robot_base import HumanLevelRobotBase
from rabbitmq_app_examle import RabbitClient
import enum


class HumanLevelHouse_EEF_ACTIONS(enum.Enum):
    LOAD = 3,
    SLEEP = 5,


class HumanLevel_GarmentAgv(HumanLevelRobotBase):

    def __init__(self, rabbit_client:RabbitClient) -> None:
        super().__init__(rabbit_client=rabbit_client)
        self.Home()
    
    def Home(self):
        self.rabbit_client.PublishToAgv('G28Z')
        self.rabbit_client.PublishToAgv('G28W')
        self.rabbit_client.PublishToAgv('M996')
        
    def Pickup_Place(self, from_where, to_where, auto_park=False):
        self.PickupFrom(from_where)
        self.PlaceTo(to_where)
        if auto_park:
            site = (2.0, 3,0)
            x,y = site.PARKING
            self.MoveTo(x,y)
        self.rabbit_client.PublishToAgv('M996')

    def MoveTo(self, height:float, angle:float):
        gcode = 'G1Z' + str(height) + 'R' + str(angle)
        self.rabbit_client.PublishToAgv(gcode)

    def DisableMotor(self):
        self.rabbit_client.PublishToAgv('M84')

    def PickupFrom(self, position_or_site):
        pass

    def PlaceTo(self, position_or_site):
        return super().PlaceTo(position_or_site)

    def EefAction(self, eef: HumanLevelHouse_EEF_ACTIONS):
        if eef==HumanLevelHouse_EEF_ACTIONS.LOAD:
            self.rabbit_client.PublishToAgv('M123P1S128')
        elif eef==HumanLevelHouse_EEF_ACTIONS.SLEEP:
            self.rabbit_client.PublishToAgv('M123P1S0')

    def Test_Eef(self):
        self.rabbit_client.PublishToAgv('M123P1S3')   #load
        self.rabbit_client.PublishToAgv('G4S5')
        self.rabbit_client.PublishToAgv('M123P1S5')   #sleep
        self.rabbit_client.PublishToAgv('G4S5')
        self.rabbit_client.PublishToAgv('M996')
        

    def demo(self):
        site = (1,2)
        x,y = site
        self.MoveTo(x,y)

        # Park Arms at a point, nearby homed position
        self.rabbit_client.PublishToAgv('G1B120F2800')
        self.rabbit_client.PublishToAgv('G1A-1F2800')
        self.rabbit_client.PublishToAgv('M996')




if __name__ == '__main__':
    from rabbitmq_app_examle import RabbitMqClient_Helper
    helper = RabbitMqClient_Helper()
    client = helper.MakeClient()
    agv_box_mover = HumanLevel_GarmentAgv(client)
    for i in range(20):
        agv_box_mover.Home()
    # agv.MoveTo(350, 60)
    # agv.DisableMotor()

    while True:
        helper.SpinOnce()