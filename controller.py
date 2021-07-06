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
        self.__current_action = bytearray([0]*14)
        self.__next_action = bytearray([0]*14)
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
            # position (x,y) of corners of chessboard
            top_left_x = -200
            top_left_y = 588.2
            bottom_right_x = 200
            bottom_right_y = 160.68

            space_x = (bottom_right_x - top_left_x) /18 
            space_y = (top_left_y - bottom_right_y) /18
            x = col_id * space_x + top_left_x
            y = row_id * space_y + bottom_right_y
            #logging.info('got_xy_from_pose_name() col_id=%d, row_id=%d',col_id,row_id)
            #logging.info('---------------- x=%d, y=%d',x,y)
        return x, y

    def convert_xy_to_4_bytes(self,x,y):
        logging.info('convert_xy() x=%d, y=%d', x, y)
        x = int(x)
        y= int(y)
        bb = x.to_bytes(2,sys.byteorder,signed=True)
        xx = bytearray(bb)
        bb = y.to_bytes(2,sys.byteorder,signed=True)
        yy = bytearray(bb)
        
        cc = bytearray([xx[0],xx[1],yy[0],yy[1]])
        print('cc=%d, %d, %d, %d', cc[0], cc[1], cc[2], cc[3])
        return cc

    def action_pickup_stone_from_cell(self, cell_name='k10'):
        print ('[Info]: action_pickup_chess_from_a_cell  %s' %cell_name)
        x,y = self.get_xy_from_pose_name(cell_name)
        cc = self.convert_xy_to_4_bytes(x,y)
        self.__next_action[2] = cc[0]
        self.__next_action[3] = cc[1]
        self.__next_action[4] = cc[2]
        self.__next_action[5] = cc[3]
        self.__next_action[0] |= 1 << 1

    def action_pickup_stone_from_warehouse(self):
        logging.info('[Info]: Action_pickup_chess_from_warehouse')
        x,y = self.get_xy_from_pose_name('origin')
        cc = self.convert_xy_to_4_bytes(x,y)
        self.__next_action[2] = cc[0]
        self.__next_action[3] = cc[1]
        self.__next_action[4] = cc[2]
        self.__next_action[5] = cc[3]
        self.__next_action[0] |= 1 << 1
    
    def action_place_stone_to_trash_bin(self, park_to_view_point=True):
        logging.info('[Info]: Action_place_chess_to_trash_bin')
        x,y = self.get_xy_from_pose_name('trash')
        cc = self.convert_xy_to_4_bytes(x,y)
        self.__next_action[6] = cc[0]
        self.__next_action[7] = cc[1]
        self.__next_action[8] = cc[2]
        self.__next_action[9] = cc[3]
        self.__next_action[0] |= 1 << 2
    
    def action_place_stone_to_cell(self, cell_name='k10', auto_park=True):
        logging.info('[Info]: action_place_chess_to_a_cell %s' %cell_name)
        x,y = self.get_xy_from_pose_name(cell_name)
        cc= self.convert_xy_to_4_bytes(x,y)
        self.__next_action[6] = cc[0]
        self.__next_action[7] = cc[1]
        self.__next_action[8] = cc[2]
        self.__next_action[9] = cc[3]
        self.__next_action[0] |= 1 << 2
                
    def action_park(self, park_cell='current'):
        '''
        finish pickup, place, and park
        The function will be blocked , until 
            (by default)ESP32 finished all actions, or
            wait blocking_time in second
        '''
        x,y = self.get_xy_from_pose_name('origin')
        cc = self.convert_xy_to4_bytes(x,y)
        self.__next_action[10] = cc[0]
        self.__next_action[11] = cc[1]
        self.__next_action[12] = cc[2]
        self.__next_action[13] = cc[3]
        self.__next_action[0] |= 1 << 3

    def home_single_arm(self, motor_id):
        '''
        motor_id == 4: Alpha
        motor_id == 5: Beta
        '''
        logging.info('home_single_arm  %d',motor_id)
        if not(motor_id == 4 or motor_id == 5):
            logging.warn('home papameter is wrong')
            return -1
        if self.__next_action[0] == 0:
            self.__next_action[0] = 1<< motor_id
        else:
            logging.warn('BLE message queue is full')

    def spin_once(self):
        action_code = self.__bleClient.read_characteristic()
        #logging.info('Controller.spin_once hardware= %d, current=%d,next=%d',action_code[0], self.__current_action[0], self.__next_action[0] )
        if int(action_code[0] / 2) == 0:
            # hardware robot is idle
            if int(self.__next_action[0] /2 ) == 0:
                # nothing to do
                return
            else:
                # Hardware robot finished last job
                self.__current_action = self.__next_action.copy()
                self.__next_action[0] = 0
                print('ble going to update...')
                #for i in range(0,13,1):
                #    print(self.__next_action[i],' ')
                self.__bleClient.write_characteristic(self.__current_action)
        else:
            # Hardware robot is busy for current action
            logging.info('Hardware robot is running task %d', self.__current_action[0])
            return

if __name__ == '__main__':
    logging.basicConfig(level=logging.DEBUG)
    tester = Controller()
    test_id = 8
    if test_id == 4:
        #home_alpha
        while True:
            for i in range(0,20,1):
                tester.home_single_arm(4)
                tester.spin_once()
                time.sleep(1)

    if test_id == 5:
        #home_beta
        while True:
            for i in range(0,20,1):
                tester.home_single_arm(5)
                tester.spin_once()
                time.sleep(1)

    if test_id == 8:
        while True:
            tester.action_pickup_stone_from_warehouse()
            tester.action_place_stone_to_cell('T1')
            tester.action_place_stone_to_trash_bin()
            for i in range(0,20,1):
                tester.spin_once()
                time.sleep(1)

    while True:
        tester.spin_once()
        time.sleep(1)
        

