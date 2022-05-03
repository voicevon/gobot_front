

import sys
sys.path.append('D:\\XumingSource\\gobot_front')  # For runing in VsCode on Windows-10 
from gogame.human_level_robot_base import HumanLevelRobotBase
from Pylib.rabbit_mq_helper import AMQ_ConnectionConfig, g_amq, MQTT_ConnectionConfig
# from rabbitmq_app_examle_uselss import RabbitClient
import enum


class HumanLevelHouse_EEF_ACTIONS(enum.Enum):
    LOAD = 3,
    SLEEP = 5,


class HumanLevel_GarmentAgv(HumanLevelRobotBase):

    def __init__(self) -> None:
        super().__init__()
        self.Home()
        self.mq_name = 'puma/agv/2134'
    
    def Home(self):
        commands = ['G28Z', 'G28W', 'M996']
        g_amq.PublishBatch(self.mq_name, commands)
        
    def Pickup_Place(self, from_where, to_where, auto_park=False):
        self.PickupFrom(from_where)
        self.PlaceTo(to_where)
        if auto_park:
            site = (2.0, 3,0)
            x,y = site.PARKING
            self.MoveTo(x,y)
        g_amq.PublishToAgv('M996')

    def MoveTo(self, height:float, angle:float):
        gcode = 'G1Z' + str(height) + 'R' + str(angle)
        g_amq.PublishToAgv(gcode)

    def DisableMotor(self):
        g_amq.PublishToAgv('M84')

    def PickupFrom(self, position_or_site):
        pass

    def PlaceTo(self, position_or_site):
        return super().PlaceTo(position_or_site)

    def EefAction(self, eef: HumanLevelHouse_EEF_ACTIONS):
        if eef==HumanLevelHouse_EEF_ACTIONS.LOAD:
            g_amq.PublishToAgv('M123P1S128')
        elif eef==HumanLevelHouse_EEF_ACTIONS.SLEEP:
            g_amq.PublishToAgv('M123P1S0')

    def Test_Eef(self):
        pause = 'G4S5'
        load = 'M123P1S3' 
        sleep = 'M123P1S5'
        commands = [load, pause, sleep, pause, 'M996']
        g_amq.PublishBatch(self.mq_name, commands)


    def demo(self):
        site = (1,2)
        x,y = site
        self.MoveTo(x,y)

        # Park Arms at a point, nearby homed position
        g_amq.PublishToAgv('G1B120F2800')
        g_amq.PublishToAgv('G1A-1F2800')
        g_amq.PublishToAgv('M996')




if __name__ == '__main__':
    c = AMQ_ConnectionConfig()
    g_amq.ConnectToRabbitMq(c)

    agv_box_mover = HumanLevel_GarmentAgv()
    for i in range(20):
        agv_box_mover.Home()
    # agv.MoveTo(350, 60)
    # agv.DisableMotor()

    while True:
        agv_box_mover.SpinOnce()