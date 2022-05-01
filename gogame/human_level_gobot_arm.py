
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

from gogame.chessboard_cell import ChessboardCell
from gogame.human_level_robot_base import HumanLevelRobotBase
from Pylib.rabbit_mq_helper import g_amq
import enum


class ArmEEF(enum.Enum):
    SLEEP = 0
    LOAD = 1
    UNLOAD = 2
    MOVE_Z_TOP = 3
    MOVE_Z_MIDDLE = 4
    MOVE_Z_BOTTOM = 5

class ArmMap():
    VENDER = (0,25)
    PARKING = (0,25)
    TRASH_WHITE = (-230, 30)
    TRASH_BLACK = (230, 30)

    @classmethod
    def CELLS(cls, cell:ChessboardCell):
        '''
        Go_position instance: "Q4" as position on 2D Chessboard.
        XY_position instance: "[0.015,0.02,-0.01]" as [x,y,z] in 3D world coordinator.
        '''
        # ss = "ABCDEFGHIJKLMNOPQRST"

        # print(cell.name)
        # iCol = ss.find(cell_name[:1])
        # iRow = 19 - int(cell_name[1:])
        zero_in_world_x = -215
        zero_in_world_y = 155
        cell_width_x = 22
        cell_width_y = 23
        arm_x = zero_in_world_x +  cell_width_x * cell.col_id
        arm_y = zero_in_world_y +  cell_width_y * cell.row_id
        return arm_x, arm_y

    def __init__(self) -> None:
        pass


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

        # self.rabbit_client.PublishToArm('G28AI')
        # self.rabbit_client.PublishToArm('G28BI')
        # self.rabbit_client.PublishToArm('M996')

    def action_pickup_stone_from_cell(self, cell:ChessboardCell):
        pass
    def action_place_stone_to_trash_bin(self):
        pass

    # TODO:  Base robot can understand RobotMap, and ArmMap, HouseMap are the subclass
    def Pickup_Place(self, from_where:ArmMap, to_where:ArmMap, auto_park=False):
        print("[Error]  human_level_gobot_arm.py Pickup_Place(), Not implicated. ")
        return
        x, y = from_where
        print("[Info] HumanLevelGobotArm::Pickup_Place from = (", str(x), "," + str(y) + ")")
        self.MoveTo(x,y)
        self.EEF_Does(ArmEEF.MOVE_Z_BOTTOM)
        self.EEF_Does(do_load=ArmEEF.LOAD)
        self.EEF_Does(do_load=ArmEEF.MOVE_Z_TOP)

        x, y = to_where
        print("[Info] HumanLevelGobotArm::Pickup_Place to = (", str(x), "," + str(y) + ")")
        self.MoveTo(x, y)
        self.EEF_Does(do_load=ArmEEF.UNLOAD)
        self.EEF_Does(do_load=ArmEEF.SLEEP)
        if auto_park:
            x,y = ArmMap.PARKING
            self.MoveTo(x, y)
        # self.rabbit_client.PublishToArm('M996')
        self.rabbit_client.Publish(self.mq_name, 'M996')


    def MoveTo(self, x:float, y:float):
        gcode = 'G1X' + str(x) + 'Y' + str(y)
        # self.rabbit_client.PublishToArm(gcode=gcode)
        self.rabbit_client.Publish(self.mq_name, gcode)
        print(gcode)

    
    def EEF_Does(self, do_load:ArmEEF):
        if do_load==ArmEEF.SLEEP:
            g_amq.Publish(self.mq_name, 'M123S0')
        elif do_load==ArmEEF.LOAD:
            g_amq.Publish(self.mq_name, 'M123S200')

    def DisableMotor(self):
        self.rabbit_client.Publish(self.mq_name, 'M84')

    def Test_Eef(self):
        commands = ['M123P1S128', 'G4S1', 'M123P1S0', 'G4S5', 'M996']
        g_amq.PublishBatch(self.mq_name, commands)

    def Calibrate_HomeAlpha(self):
        pause = "G4S5"
        commands = ['G28AI', 'G1A-6.283', pause, 'G1A-1.571', pause, 'M996']
        g_amq.PublishBatch(self.mq_name, commands)

    def Calibrate_HomeBeta(self):
        pause = "G4S5"
        commands = ['G28BI', 'G1B3.142', pause, 'G1B1.571', pause, 'M996']
        g_amq.PublishBatch(self.mq_name, commands)

if __name__ == '__main__':
    arm = HumanLevelGobotArm(robot_serial_id=2134, do_home=False)
    arm.Calibrate_HomeAlpha()
    arm.Calibrate_HomeBeta()

    if False:
        # Test Map
        from_where = ArmMap.VENDER
        to_cell = ChessboardCell()
        to_cell.from_name("A1")
        to_where = ArmMap.CELLS(to_cell)
        arm.Pickup_Place(from_where=from_where, to_where=to_where)

    if False:
        # Test rabbitMQ

        for i in range(200):
            arm.Test_Eef()

        while True:
            arm.SpinOnce()