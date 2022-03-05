
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
sys.path.append('C:\\gitlab\\gobot_front')  # For runing in VsCode on Windows-10 

from gogame.chessboard_cell import ChessboardCell
from gogame.human_level_robot_base import HumanLevelRobotBase
from rabbitmq_all_in_one import RabbitClient
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
    def CELLS(self, cell:ChessboardCell):
        '''
        Go_position instance: "Q4" as position on 2D Chessboard.
        XY_position instance: "[0.015,0.02,-0.01]" as [x,y,z] in 3D world coordinator.
        '''
        ss = "ABCDEFGHIJKLMNOPQRST"

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

    def __init__(self, rabbit_client:RabbitClient) -> None:
        super().__init__(rabbit_client=rabbit_client)
        self.Home()

    def Home(self):
        self.rabbit_client.PublishToArm('G28AI')
        self.rabbit_client.PublishToArm('G28BI')
        self.rabbit_client.PublishToArm('M996')

    
    def Pickup_Place(self, from_where, to_where, auto_park=False):
        x, y = from_where
        self.MoveTo(x,y)
        self.EEF_Does(ArmEEF.MOVE_Z_BOTTOM)
        self.EEF_Does(do_load=ArmEEF.LOAD)
        self.EEF_Does(do_load=ArmEEF.MOVE_Z_TOP)
        x, y = to_where
        self.MoveTo(x, y)
        self.EEF_Does(do_load=ArmEEF.UNLOAD)
        self.EEF_Does(do_load=ArmEEF.SLEEP)
        if auto_park:
            x,y = ArmMap.PARKING
            self.MoveTo(x, y)
        self.rabbit_client.PublishToArm('M996')

    # def PickupFrom(self, position_or_site):
    #     return super().PickupFrom(position_or_site)

    # def PlaceTo(self, position_or_site):
    #     return super().PlaceTo(position_or_site)

    def MoveTo(self, x:float, y:float):
        gcode = 'G1X' + str(x) + 'Y' + str(y)
        self.rabbit_client.PublishToArm(gcode=gcode)

    
    def EEF_Does(self, do_load:ArmEEF):
        # Load, Unload, Sleep
        pass



    # def SencePoint_ToXY(self, position):
    #     if position=='trash_bin':
    #         x = 123
    #         y = 125
    #     elif position=='house':
    #         x= 11.2
    #         y=22.3
    #     else:
    #         MessageLogger.Output('convert_to_world_position', position)
    #         x, y = self.Convert_to_world_position(position)
    #     MessageLogger.Output('getxy', position + '  ' + str(x) + '  ' + str(y))

    #     return x,y

    # def get_next(self)->str:
    #     row = self.test_cell.row_id
    #     col = self.test_cell.col_id
    #     col += 1
    #     if col >=19:
    #         col = 0
    #         row += 1
    #         if row >= 19:
    #             row = 0

    #     self.test_cell.from_col_row_id(col_id=col, row_id=row)
    #     return self.test_cell.name

    def Test_Eef(self):
        # self.rabbit_client.PublishToHouse('M123P1S128')
        # self.rabbit_client.PublishToHouse('G4S1')
        # self.rabbit_client.PublishToHouse('M123P1S0')
        # self.rabbit_client.PublishToHouse('G4S5')
        # self.rabbit_client.PublishToHouse('M996')
        
        self.rabbit_client.PublishToArm('M123P1S128')
        self.rabbit_client.PublishToArm('G4S1')
        self.rabbit_client.PublishToArm('M123P1S0')
        self.rabbit_client.PublishToArm('G4S5')
        self.rabbit_client.PublishToArm('M996')


if __name__ == '__main__':
    from rabbitmq_all_in_one import RabbitMqClient_Helper
    helper = RabbitMqClient_Helper()
    client = helper.MakeClient()
    arm = HumanLevelGobotArm(client)

    if True:
        # Test Map
        from_where = ArmMap.VENDER
        to_cell = ChessboardCell()
        to_cell.from_name("A1")
        to_where = ArmMap.CELLS(to_cell)
        arm.Pickup_Place(from_where=from_where, to_where=to_cell)

    if False:
        # Test rabbitMQ

        for i in range(200):
            arm.Test_Eef()

        while True:
            arm.SpinOnce()