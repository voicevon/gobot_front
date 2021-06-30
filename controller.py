from config import config as  app_config
from ble_client import BleClient
import logging
import sys
import time
sys.path.append("/home/pi/pylib")
from terminal_font import TerminalFont

#   typedef union {
#     struct {
#       uint8_t action_code;
#       // uint8_t pickup_type;   // 0=skip, 1=do it.
#       int16_t pickup_x;
#       int16_t pickup_y;
#       // uint8_t place_type;    // 0=skip, 1=lower release, 2=higher release
#       int16_t place_x;
#       int16_t place_y;
#       // uint8_t do_park;       //0= skip, 1=do it.
#       int16_t park_x;
#       int16_t park_y;
#     }Arm;
#     struct{
#       uint8_t action_code;
#       uint16_t from_start_house_id;  //From or start house id
#     }House;
#     byte bytes[13];
#   }BodyAction;
  

class Controller:
    '''
    This is an abstract robot.
    It is a  thing that Understood coordinators, 
    and can tranlate from chessboard co-ord to robot arm co-ord
        1. Chessboard coordinator: like 'Q4', 'T19', 
            Some position is out of the chessboard, like 'Trash','HouseEnd' 
        2. Robot arm coordinator: (35,126) (258,129)

    '''
    def __init__(self):
        '''
        '''
        self._FC_YELLOW = TerminalFont.Color.Fore.yellow
        self._BG_GREEN = TerminalFont.Color.Background.green
        self._FC_RESET = TerminalFont.Color.Control.reset 
        self.__current_action = bytearray([0]*13)
        self.__next_action = bytearray([0]*13)
        self.__bleClient = BleClient()

    def get_xy_from_pose_name(self, pos_name='origin'):
        # origin point
        x = 0
        y = 100
        if pos_name == 'origin':
            pass
        elif pos_name == 'trash':
            x = 500
            y = 200
        else:
            # cell name , like 'Q4'
            col_name_list = 'ABCDEFGHJKLMNOPQRST'
            col_id = 18 - col_name_list.find(pos_name[:1])
            row_id = int(pos_name[1:]) -1    # exeption here:
            x = col_id * 22 + 12
            y = row_id * 23 + 45
        return x, y

    def action_pickup_stone_from_cell(self, cell_name='k10'):
        print ('[Info]: action_pickup_chess_from_a_cell  %s' %cell_name)
        x,y = self.get_xy_from_pose_name(cell_name)
        self.__next_action[1] = x /256
        self.__next_action[2] = x % 256
        self.__next_action[3] = y / 256
        self.__next_action[4] = y % 256
        self.__next_action[0] = 1 << 3

    def action_pickup_stone_from_warehouse(self):
        logging.info('[Info]: Action_pickup_chess_from_warehouse')
        x,y = self.get_xy_from_pose_name('origin')
        self.__next_action[1] = int(x /256)
        self.__next_action[2] = x % 256
        self.__next_action[3] = int(y / 256)
        self.__next_action[4] = y % 256
        self.__next_action[0] = 1 << 3
    
    def action_place_stone_to_trash_bin(self, park_to_view_point=True):
        logging.info('[Info]: Action_place_chess_to_trash_bin')

        x,y = self.get_xy_from_pose_name('trash')
        self.__next_action[5] = int(x /256)
        self.__next_action[6] = x % 256
        self.__next_action[7] = int(y / 256)
        self.__next_action[8] = y % 256
        self.__next_action[0] = 1 << 3
    
    def action_place_stone_to_cell(self, cell_name='k10', auto_park=True):
        logging.info('[Info]: action_place_chess_to_a_cell %s' %cell_name)
        x,y = self.get_xy_from_pose_name(cell_name)
        self.__next_action[5] = int(x /256)
        self.__next_action[6] = x % 256
        self.__next_action[7] = int(y / 256)
        self.__next_action[8] = y % 256
        self.__next_action[0] = 1 << 3
                
    def action_park(self, park_cell='current', blocking_time= 0):
        '''
        finish pickup, place, and park
        The function will be blocked , until 
            (by default)ESP32 finished all actions, or
            wait blocking_time in second
        '''
        x,y = self.get_xy_from_pose_name('origin')
        self.__next_action[9] = int(x /256)
        self.__next_action[10] = x % 256
        self.__next_action[11] = int(y / 256)
        self.__next_action[12] = y % 256
        self.__next_action[0] = 1 << 3

        # Wait until the current action is finished
        while self.__current_action[0] >= 2:
            if blocking_time == 0:
                print(' current action to be updated from ESP-Controller, self.__current_action[0]=', self.__current_action)
                # if blocking_time == 0:
                pass
            else:
                time.sleep(blocking_time)
                return;

        # Current action is finsihed, hardware is idle
        # copy next to current
        for i in range(0,13,1):
            self.__current_action[i] = self.__next_action[i]
        # self.__current_action = self.__next_action
        self.__next_action[0] == 0
        #self.__bleClient.update_characteristic(self.__current_action)
        print('ccccccccccccccccccccccccccccccc, bleClient.updated...')


        
