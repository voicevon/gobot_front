
#                        ^
#      A1                |            A19
#     (-200.25, 589.75)  |            (200.25, 589.75)
#                        |
#                        |
#                        |
#                        |
#      T1                |            T19
#     (-200.25, 161.25)  |            (200.25, 161.25)
#                        Head(0,88)
#                        |
#------------O---------(0,0)---------O-------------->X

# from config.message_logger import MessageLogger

import sys
sys.path.append('D:\\XumingSource\\gobot_front')  # For runing in VsCode on Windows-10 

from Pylib.robot_map import MapSite
from gogame.arm_map import ArmMapSite_Catalog, ArmMapSiteFactory
from gogame.chessboard_cell import ChessboardCell
from gogame.human_level_robot_base import HumanLevelRobotBase
from Pylib.rabbit_mq_helper import AMQ_ConnectionConfig, g_amq
import enum


class ArmEEF(enum.Enum):
    SLEEP = 0
    LOAD = 1
    UNLOAD = 2
    MOVE_Z_TOP = 3
    MOVE_Z_MIDDLE = 4
    MOVE_Z_BOTTOM = 5


class HumanLevelGobotArm(HumanLevelRobotBase):

    def __init__(self,robot_serial_id:int, do_home=True) -> None:
        super().__init__()
        mq_name = 'gobot_xnnnn_arm'
        self.mq_name = mq_name.replace('nnnn', str(robot_serial_id))
        if do_home:
            self.HomeAaphaBeta()

    def HomeAaphaBeta(self):
        print('[Info] HumanLevelGobotArm::Home() ')
        commands = ['G28AI','G28BI', 'M996']
        g_amq.PublishBatch(self.mq_name, commands)

    def action_pickup_stone_from_cell(self, cell:ChessboardCell):
        pass

    def action_place_stone_to_trash_bin(self):
        pass

    # TODO:  Base robot can understand RobotMap, and ArmMap, HouseMap are the subclass
    def Pickup_Place(self, from_where:MapSite, to_where:MapSite, auto_park=False):
        # print("[Error]  human_level_gobot_arm.py Pickup_Place(), Not implicated. ")
        # return
        print("[Info] HumanLevelGobotArm::Pickup_Place from = (", from_where.Name + ")")
        self.MoveTo(from_where)
        self.EEF_Does(ArmEEF.MOVE_Z_BOTTOM)
        self.EEF_Does(do_load=ArmEEF.LOAD)
        self.EEF_Does(do_load=ArmEEF.MOVE_Z_TOP)

        print("[Info] HumanLevelGobotArm::Pickup_Place to = (", to_where.Name + ")")
        self.MoveTo(to_where)
        self.EEF_Does(do_load=ArmEEF.UNLOAD)
        self.EEF_Does(do_load=ArmEEF.SLEEP)
        if auto_park:
            parking_at = ArmMapSiteFactory().MakeSingleSite(ArmMapSite_Catalog.PARKING)
            self.MoveTo(parking_at)
        g_amq.Publish(self.mq_name, 'M996')


    def MoveTo(self, site:MapSite ):
        x = site.X
        y = site.Y
        gcode = 'G1X' + str(x) + 'Y' + str(y)
        g_amq.Publish(self.mq_name, gcode)
        print(gcode)

    
    def EEF_Does(self, do_load:ArmEEF):
        if do_load==ArmEEF.SLEEP:
            g_amq.Publish(self.mq_name, 'M123S0')
        elif do_load==ArmEEF.LOAD:
            g_amq.Publish(self.mq_name, 'M123S200')

    def DisableMotor(self):
        g_amq.Publish(self.mq_name, 'M84')

    def Test_Eef(self):
        commands = ['M123P1S128', 'G4S1', 'M123P1S0', 'G4S5', 'M996']
        g_amq.PublishBatch(self.mq_name, commands)

    def Calibrate_1_HomeAlpha(self):
        '''
        Home, (AntiClockwise)-180, pause, -90, pause
        '''
        pause = "G4S5"
        commands = ['G28Bi','G28AI', 'G1A180', pause, 'G1A90', pause, 'G1A150','M996']
        g_amq.PublishBatch(self.mq_name, commands)

    def Calibrate_2_HomeBeta(self):
        '''
        Home, (Clockwise) 180, pause, 90, pause 
        '''
        pause = "G4S1"
        commands = ['G28AI','G1A180','G28BI', 'G1B0', pause, 'G1B90', pause, 'G1B30', 'M996']
        g_amq.PublishBatch(self.mq_name, commands)
        
    def Calibrate_3_HomeAlphaBeta(self):
        pause = "G4S3"
        commands = ['G28AI','G1A180','G28BI', 'G1B0', pause, 'G1A180B90', pause, 'G1A90B0',pause, 'G1A150B30', 'M996']
        g_amq.PublishBatch(self.mq_name, commands) 


    def Calibrate_4_ARM_LINE(self):
        pause = "G4S10"
        commands = ['G28AI','G1A180','G28BI', 'G1B0', pause, 'G1A150B30', 'M996']
        g_amq.PublishBatch(self.mq_name, commands) 

    def Calibrate_12_ZigZag_X(self):
        # 'Home, Move to A1, pause, Move to T19, pause'
        pause = "G4S2"
        self.HomeAaphaBeta()
        cell_names=['A1', 'K1', 'T1', 'A10','K10','T10','A19','K19','T19','K1']
        for i in range(1):
            for cell_name in cell_names:
                cell = ChessboardCell()
                cell.from_name(cell_name)
                cell_site = ArmMapSiteFactory().MakeSingleSite(ArmMapSite_Catalog.CHESSBOARD_CELL, cell)
                commands = [ 'G1X' + str(cell_site.X) + "Y" + str(cell_site.Y)
                        ,pause
                        ]
                print(commands)
                g_amq.PublishBatch(self.mq_name, commands)
        g_amq.Publish(self.mq_name, 'M996')

    def Calibrate_19_A1_only(self):
        # 'Home, Move to A1, pause, Move to T19, pause'
        pause = "G4S2"
        self.HomeAaphaBeta()
        cell_names=['A1']
        for i in range(1):
            for cell_name in cell_names:
                cell = ChessboardCell()
                cell.from_name(cell_name)
                cell_site = ArmMapSiteFactory().MakeSingleSite(ArmMapSite_Catalog.CHESSBOARD_CELL, cell)
                commands = [ 'G1X' + str(cell_site.X) + "Y" + str(cell_site.Y)
                        ,pause
                        ]
                print(commands)
                g_amq.PublishBatch(self.mq_name, commands)
        g_amq.Publish(self.mq_name, 'M996')

    def Calibrate_11_ZigZag_Y(self):
        'Home, Move to A1, pause, Move to T19, pause'
        pause = "G4S2"
        self.HomeAaphaBeta()
        vender = ArmMapSiteFactory().MakeSingleSite(ArmMapSite_Catalog.HOUSE_VENDOR)
        commands = [ 'G1X' + str(vender.X) + "Y" + str(vender.Y) , pause]
        g_amq.PublishBatch(self.mq_name, commands)
        cell_names=['A1', 'A10', 'A19', 'K19','K10','K1','T1','T10','T19','K1']
        for i in range(1):
            for cell_name in cell_names:
                cell = ChessboardCell()
                cell.from_name(cell_name)
                cell_site = ArmMapSiteFactory().MakeSingleSite(ArmMapSite_Catalog.CHESSBOARD_CELL, cell)
                commands = [ 'G1X' + str(cell_site.X) + "Y" + str(cell_site.Y) + "F6000"
                        ,pause
                        ]
                print(commands)
                g_amq.PublishBatch(self.mq_name, commands)
        g_amq.Publish(self.mq_name, 'M996')
    
    def Calibrate_98_Lock_Alpha(self):
        g_amq.Publish(self.mq_name, "G28AI")

    def Calibrate_98_Lock_Beta(self):
        g_amq.Publish(self.mq_name, "G28BI")

    def Calibrate_99_Motor_gear_ratio(self):
        pause = "G4S1"
        g_amq.Publish(self.mq_name, "G28BI")
        g_amq.Publish(self.mq_name, "G4S10")

        for i in range(20):
            a = 360 * i
            g_amq.Publish(self.mq_name, "G1B" + str(a))
            g_amq.Publish(self.mq_name, pause)

if __name__ == '__main__':
    config = AMQ_ConnectionConfig()
    g_amq.ConnectToRabbitMq(config)

    arm = HumanLevelGobotArm(robot_serial_id=2134, do_home=False)
    # arm.Calibrate_99_Motor_gear_ratio()
    # arm.Calibrate_98_Lock_Alpha()
    # arm.Calibrate_98_Lock_Beta()


    # arm.Calibrate_1_HomeAlpha()
    # arm.Calibrate_2_HomeBeta()
    # arm.Calibrate_3_HomeAlphaBeta()
    # arm.Calibrate_4_ARM_LINE()
    arm.Calibrate_11_ZigZag_Y()
    # arm.Calibrate_12_ZigZag_X()
    # arm.Calibrate_19_A1_only()

    if False:
        # Test Map
        from_where = ArmMapSiteFactory().MakeSingleSite(ArmMapSite_Catalog.HOUSE_VENDOR)
        to_cell = ChessboardCell()
        to_cell.from_name("A1")

        to_site = ArmMapSiteFactory().MakeSingleSite(ArmMapSite_Catalog.CHESSBOARD_CELL,to_cell)
        arm.Pickup_Place(from_where=from_where, to_where=to_site)

    if False:
        # Test rabbitMQ

        for i in range(200):
            arm.Test_Eef()

        while True:
            arm.SpinOnce()