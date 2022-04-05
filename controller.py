
# from commuDevice.ble_single_client import BleSingleClient, BleServerHead
import time


if __name__ == '__main__':
    from rabbitmq_app_examle import RabbitMqClient_Helper
    from gogame.human_level_gobot_house import HumanLevelGobotHouse
    from gogame.human_level_gobot_arm import HumanLevelGobotArm

    
    helper = RabbitMqClient_Helper()
    client = helper.MakeClient()
    arm = HumanLevelGobotArm(client)
    house = HumanLevelGobotHouse(client)
    house.Home()

    while True:
        helper.SpinOnce()
        # house.SpinOnce()
        # arm.MoveTo(arm.get_next())
        # time.sleep(1)

# class Controller:
#     '''
#     This is an abstract robot.
#     It is a  thing that Understood coordinators, 
#     and can tranlate from chessboard co-ord to robot arm co-ord
#         1. Chessboard coordinator: like 'Q4', 'T19', 
#             Some position is out of the chessboard, like 'Trash','HouseEnd' 
#         2. Robot arm coordinator: (35,126) (258,129)

#     This class will orgnize data, then send data to 
#     1. MQTT broker or
#     2. TCP client or
#     3. BLE peripheral
#     '''
#     def __init__(self):
#         '''
#         '''
#         self.__current_action = bytearray([0]*14)
#         self.__next_action = bytearray([0]*14)
#         # the_head= BleServerHead()
#         # self.__bleClient = BleSingleClient(the_head)

#         self.Location_A1 = (-200.25, 589.75)
#         self.Location_T19 = (200.25, 161.25)
#         self.Location_origin = (0, 88)
#         self.rabbitMqClient = RabbitMqClient_Helper.MakeClient()

#         # self.ARM_AXIS_Alpha = 4
#         # self.ARM_AXIS_Beta = 5
#         # self.HOUSE_AXIS_Alpha = 10
#         # self.HOUSE_AXIS_Beta = 11

#         self._FC_YELLOW = TerminalFont.Color.Fore.yellow
#         self._BG_GREEN = TerminalFont.Color.Background.green
#         self._FC_RESET = TerminalFont.Color.Control.reset 
#         logging.info('Init controller is done......')


#     def get_xy_from_pose_name(self, pos_name='origin'):
#         # origin point
#         x = 0
#         y = 100
#         if pos_name == 'origin':
#             pass
#         elif pos_name == 'trash':
#             x = 400
#             y = 200
#         else:
#             # cell name , like 'Q4'
#             col_name_list = 'ABCDEFGHJKLMNOPQRST'
#             col_id = 18 - col_name_list.find(pos_name[:1])
#             row_id = int(pos_name[1:]) -1    # exeption here:
#             logging.info(" col_id =%d, row_id=%d", col_id, row_id)
#             # position (x,y) of corners of chessboard
#             top_left_x = -200.25
#             top_left_y = 589.75
#             bottom_right_x = 200.25
#             bottom_right_y = 161.25

#             space_x = (bottom_right_x - top_left_x) /18 
#             space_y = (top_left_y - bottom_right_y) /18
#             x = col_id * space_x + top_left_x
#             y = row_id * space_y + bottom_right_y
#             #logging.info('got_xy_from_pose_name() col_id=%d, row_id=%d',col_id,row_id)
#             #logging.info('---------------- x=%d, y=%d',x,y)
#         return x, y

#     def convert_xy_to_4_bytes(self,x,y):
#         #logging.info('convert_xy() x=%d, y=%d', x, y)
#         x = int(x)
#         y= int(y)
#         bb = x.to_bytes(2,sys.byteorder,signed=True)
#         xx = bytearray(bb)
#         bb = y.to_bytes(2,sys.byteorder,signed=True)
#         yy = bytearray(bb)
        
#         cc = bytearray([xx[0],xx[1],yy[0],yy[1]])
#         #print('cc=%d, %d, %d, %d', cc[0], cc[1], cc[2], cc[3])
#         return cc

#     def action_pickup_stone_from_cell(self, cell_name='k10'):
#         print ('[Info]: action_pickup_chess_from_a_cell  %s' %cell_name)
#         x,y = self.get_xy_from_pose_name(cell_name)
#         gcode = "G1X" + str(x) + "Y" + str(y)
#         self.rabbitMqClient.PublishToArm(gcode=gcode)

#         # cc = self.convert_xy_to_4_bytes(x,y)
#         # self.__next_action[2] = cc[0]
#         # self.__next_action[3] = cc[1]
#         # self.__next_action[4] = cc[2]
#         # self.__next_action[5] = cc[3]
#         # self.__next_action[0] |= 1 << 1

#     def action_pickup_stone_from_warehouse(self):
#         logging.info('  action_pickup_chess_from_warehouse')
#         x,y = self.get_xy_from_pose_name('origin')
#         gcode = "G1X" + str(x) + "Y" + str(y)
#         self.rabbitMqClient.PublishToArm(gcode=gcode)
#         # cc = self.convert_xy_to_4_bytes(x,y)
#         # self.__next_action[2] = cc[0]
#         # self.__next_action[3] = cc[1]
#         # self.__next_action[4] = cc[2]
#         # self.__next_action[5] = cc[3]
#         # self.__next_action[0] |= 1 << 1
    
#     def action_place_stone_to_trash_bin(self, park_to_view_point=True):
#         logging.info('   action_place_chess_to_trash_bin')
#         x,y = self.get_xy_from_pose_name('trash')
#         gcode = "G1X" + str(x) + "Y" + str(y)
#         self.rabbitMqClient.PublishToArm(gcode=gcode)

#         # cc = self.convert_xy_to_4_bytes(x,y)
#         # self.__next_action[6] = cc[0]
#         # self.__next_action[7] = cc[1]
#         # self.__next_action[8] = cc[2]
#         # self.__next_action[9] = cc[3]
#         # self.__next_action[0] |= 1 << 2
    
#     def action_place_stone_to_cell(self, cell_name='k10', auto_park=True):
#         logging.info('  action_place_chess_to_a_cell %s' %cell_name)
#         x,y = self.get_xy_from_pose_name(cell_name)
#         gcode = "G1X" + str(x) + "Y" + str(y)
#         self.rabbitMqClient.PublishToArm(gcode=gcode)

#         # cc= self.convert_xy_to_4_bytes(x,y)
#         # self.__next_action[6] = cc[0]
#         # self.__next_action[7] = cc[1]
#         # self.__next_action[8] = cc[2]
#         # self.__next_action[9] = cc[3]
#         # self.__next_action[0] |= 1 << 2
                
#     def action_park(self, park_cell='current'):
#         '''
#         finish pickup, place, and park
#         The function will be blocked , until 
#             (by default)ESP32 finished all actions, or
#             wait blocking_time in second
#         '''
#         logging.info('  action_park() To %s', park_cell)
#         x,y = self.get_xy_from_pose_name('origin')
#         cc = self.convert_xy_to_4_bytes(x,y)
#         self.__next_action[10] = cc[0]
#         self.__next_action[11] = cc[1]
#         self.__next_action[12] = cc[2]
#         self.__next_action[13] = cc[3]
#         self.__next_action[0] |= 1 << 3

#     def home_single_axis(self, axis_id):
#         '''
#         axis_id == 4: Arm.Alpha
#         axid_id == 5: Arm.Beta
#         axis_id == 10: House.Alpha
#         axid_id == 11: House.Beta
#         '''
#         logging.info('home_single_axis  %d', axis_id)
#         if self.__next_action[0] == 0:
#             if axis_id == self.ARM_AXIS_Alpha or axis_id == self.ARM_AXIS_Beta:
#                 self.__next_action[0] = 1<< axis_id
#                 self.__next_action[1] = 0
#             elif axis_id == self.HOUSE_AXIS_Alpha or axis_id == self.HOUSE_AXIS_Beta:
#                 self.__next_action[0] = 0
#                 self.__next_action[1] = 1<< (axis_id - 8)
#             else:
#                 logging.warn('home papameter is wrong')
#                 return -1
#         else:
#             logging.warn('BLE message queue is full')

#     def spin_once(self):
#         self.rabbitMqClient.SpinOnce()
#         return

#         # # action_code = self.__bleClient.read_characteristic()
#         # action_code = [1,2,3,3,4,5,6,7,8,9,11]
#         # #logging.info('Controller.spin_once hardware= %d, current=%d,next=%d',action_code[0], self.__current_action[0], self.__next_action[0] )
#         # if int(action_code[0] / 2) == 0:
#         #     # hardware robot is idle
#         #     if int(self.__next_action[0] /2 ) == 0:
#         #         # nothing to do
#         #         return
#         #     else:
#         #         # Hardware robot finished last job
#         #         self.__current_action = self.__next_action.copy()
#         #         self.__next_action[0] = 0
#         #         print('ble going to update...')
#         #         #for i in range(0,13,1):
#         #         #    print(self.__next_action[i],' ')
#         #         # self.__bleClient.write_characteristic(self.__current_action)
#         # else:
#         #     # Hardware robot is busy for current action
#         #     logging.info('Hardware robot is running task %d', self.__current_action[0])
#         #     return

# if __name__ == '__main__':
#     logging.basicConfig(level=logging.DEBUG)
#     tester = Controller()
#     test_id = 8

#     if test_id == 4:
#         #home  do this homing repeatly.
#         home_axis = 4 # 4, 5,10, 11
#         while True:
#             for target_axis in [4,5,10,11]:
#                 tester.home_single_axis(target_axis)
#                 tester.spin_once()
#                 time.sleep(1)

#     if test_id == 8:
#         while True:
#             #tester.action_pickup_stone_from_warehouse()
#             tester.action_pickup_stone_from_cell('K10')
#             tester.action_place_stone_to_cell('T1')
#             #tester.action_place_stone_to_trash_bin()
#             tester.action_park()
#             for i in range(0,20,1):
#                 tester.spin_once()
#                 time.sleep(1)

        

