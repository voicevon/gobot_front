# from vision.grid_finder import CommandFinder
import cv2
import numpy as np
#import threading
#import time  # only for sleep


# from gogame.chessboard import ChessboardLayout, DiedAreaScanner
# from config import config, CvDebugger
# from terminal_font import TerminalFont


#from mark_scanner import MarkScanner
#from board_scanner import BoardScanner
#from layout_scanner import LayoutScanner
from vision.robot_eye import MonoEye
# from vision.aruco_finder import ArucoFinder
# from vision.grid_finder import Commander
from gobot_vision.gobot_vision import GobotVision
from config import config

import sys
sys.path.append("/home/pi/pylib")
from terminal_font import TerminalFont
from mqtt_helper import g_mqtt

class AiClient():
    pass


class GobotHead():

    def __init__(self):
        self.__eye = MonoEye('2021-0611.yml')
        self.__vision = GobotVision()

        self.__goto = self.at_state_game_over


        self.__FC_YELLOW = TerminalFont.Color.Fore.yellow
        self.__BG_BLUE = TerminalFont.Color.Background.blue
        self.__FC_RESET = TerminalFont.Color.Control.reset
        self.__MARK_STABLE_DEPTH = 5
        self.__LAYOUT_STABLE_DEPTH = 5


    def get_stable_layout(self,min_stable_depth):
        stable_depth = 0
        while stable_depth < min_stable_depth:
            ret, img_origin = self.__capture_newest_image()
            if ret:
                # got origin image
                # img_board = self.__board_scanner.get_whole_area_of_chessboard(img_origin)
                img_board = self.__board_scanner.get_whole_area_of_chessboard_new_idea(img_origin)
                if img_board is not None:
                    # got board image
                    layout, stable_depth = self.__layout_scanner.start_scan(img_board, self.__LAYOUT_STABLE_DEPTH)
        layout.rename_to('stable detect layout (depth = %d)' % stable_depth)
        return layout

    def get_chessboard_test(self):
        ret, img = self.__capture_newest_image()
        if ret:
            self.new_idea(img)

    def get_stable_mark(self,min_stable_depth):
        stable_depth = 0
        while stable_depth < min_stable_depth:
            ret, img = self.__capture_device.read()
            if ret:
                mark_index, stable_depth = self.__mark_scanner.detect_mark(img, min_stable_depth)
        return mark_index
        
    def start_show(self,eye_name):
        xx = self.__cvWindow.get_all_windows()[eye_name]
        xx[0] = True
        # self.__thread_eye = threading.Thread(target=self.__robot_eye.monitor)
        if self.__thread_eyes.get(eye_name) == None:
            self.__thread_eyes[eye_name] = threading.Thread(target=self.__start_show, args=[eye_name])
            self.__thread_eyes[eye_name].start()
            
    def stop_show(self, eye_name):
        # this_window = self.__cvWindow.get_window(window_name)
        self.__cvWindow.get_all_windows()[eye_name][0] = False

    def user_playing(self):
        self.go_agent.send("move c4 B")
    
    def __remove_one_cell_to_trash(self, color):
        '''
        return: how many chesses have benn removed
            0 = zero chess has been removed
            1 = one chess has been removed
        '''
        the_layout = self.__eye.get_stable_layout(self.__LAYOUT_STABLE_DEPTH)
        the_layout.print_out()
        cell = the_layout.get_first_cell(color)
        if cell is not None:
            print (self.__FC_GREEN + '[INFO]: GoManager.__remove_one_cell_to_trash() ' + cell.name + self.__FC_RESET)
            # move the first chess cell to trash
            # cell = ChessboardCell()
            # cell.from_col_row_id(col_id=col, row_id=row)
            self.__arm.action_pickup_chess_from_a_cell(cell.name)
            self.__arm.action_place_chess_to_trash_bin()
            return 1
        return 0

    def __remove_died_area_(self, target_color_code):
        '''
        Will do:
            1. check died area
            2. remove died cells on __ai_layout
        Will not do:
            3. remove cells phisically with robot arm
        '''
        died_area_helper = DiedAreaDetector()
        area = died_area_helper.start(self.__ai_go.layout.get_layout_array(), target_color_code)
        for col in range(0,19):
            for row in range(0,19):
                if area[col][row] == 0:
                    cell = ChessboardCell()
                    cell.from_col_row_id(col,row)
                    # self.__arm.action_pickup_chess_from_a_cell(cell.name)
                    # self.__arm.action_place_chess_to_trash_bin()
                    self.__ai_go.layout.play_col_row(col,row,self.__BLANK)

    def at_state_begin(self):
        # scan the marks, to run markable command
        command = self.__eye.get_stable_mark(self.__MARK_STABLE_DEPTH)

        if command == 4:
            self.__ai_go.start_new_game()
            self.__mqtt.publish('gogame/smf/status', 'computer_playing', retain=True)
            self.__goto = self.at_state_computer_play
        else:
            print(self.__FC_YELLOW + '[Warning]: GoManger.at_begining()  scanned command=%d' %command)
            self.__goto = self.at_state_game_over

    def at_state_game_over(self):
        # scan the marks, to run markable command
        # command = self.__eye.get_stable_mark(self.__MARK_STABLE_DEPTH)

        command = self.__vision.get_commander(self.__last_image)
        # ).__command_finder.get_command_from_image(self.__last_image)
        # image = self.__vision.get_commander_plate(self.__last_image)


        if command == 0:
            self.__goto = self.at_demo_from_warehouse
        
        if command == 1:
            self.__goto = self.at_demo_mover

        if command == 2:
            self.__goto = self.at_demo_remove_to_trashbin_black
  
        elif command == 3:
            self.__goto = self.at_demo_remove_to_trashbin_white

        elif command == 4:
            self.__goto = self.at_state_begin
            self.__mqtt.publish('gogame/smf/status','begining',retain=True)
        else:
            print(self.__FC_YELLOW + '[Warning]: GoManger.at_begining()  scanned command=%d' %command)

    def at_state_computer_play(self):
        self.__ai_go.get_final_score()
        # get command from PhonixGo
        cell_name = self.__ai_go.get_ai_move()
        if cell_name is not None:
            # some time the ai_player will return a 'resign' as a cell name.
            logging.info(self.__FC_PINK + 'AI step: place black at: %s' %cell_name + self.__FC_RESET)
            # robot arm play a chess, The instruction is from AI.
            self.__arm.action_pickup_chess_from_warehouse()
            self.__arm.action_place_chess_to_a_cell(cell_name=cell_name)
            self.__ai_go.layout.play(cell_name, self.__BLACK)

        self.__goto = self.at_state_scan_died_white

    def at_state_withdraw_white(self):
        cell = self.__died_area_scanner.get_first_died_cell()
        if cell is None:
            # There is no died area to be removed
            self.__goto = self.at_state_compare_layout_black
        else:
            # only remove one cell of the died area.
            # will go on to remove other cells on the next invoking
            self.__arm.action_pickup_chess_from_a_cell(cell.name)
            self.__arm.action_place_chess_to_trash_bin()
            self.__ai_go.layout.play(cell.name, self.__BLANK)
            self.__died_area_scanner.died_cell_removed_first_one()

    def at_state_withdraw_black(self):
        cell = self.__died_area_scanner.get_first_died_cell()
        if cell is None:
            # There is no died area to be removed
            self.__goto = self.at_state_compare_layout_white
        else:
            # only remove one cell of the died area.
            # will go on to remove other cells on the next invoking
            self.__arm.action_pickup_chess_from_a_cell(cell.name)
            self.__arm.action_place_chess_to_trash_bin()
            self.__ai_go.layout.play(cell.name, self.__BLANK)
            self.__died_area_scanner.died_cell_removed_first_one()

    def at_state_scan_died_black(self):
        self.__died_area_scanner.set_layout_array(self.__ai_go.layout.get_layout_array())
        count = self.__died_area_scanner.start_scan(self.__BLACK)
        if count > 0:
            self.__died_area_scanner.print_out_died_area()

        self.__goto = self.at_state_withdraw_black

    def at_state_scan_died_white(self):
        self.__died_area_scanner.set_layout_array(self.__ai_go.layout.get_layout_array())
        count = self.__died_area_scanner.start_scan(self.__WHITE)
        if count > 0:
            self.__died_area_scanner.print_out_died_area()
        self.__goto = self.at_state_withdraw_white

    def at_state_compare_layout_white(self):
        layout = self.__eye.get_stable_layout(self.__LAYOUT_STABLE_DEPTH)
        diffs = self.__ai_go.layout.compare_with(layout)
        if len(diffs) == 0:
            self.__goto = self.at_state_computer_play
        else: 
            diffs = self.__ai_go.layout.compare_with(layout, do_print_out=True)
            time.sleep(10)

    def at_state_compare_layout_black(self):
        layout = self.__eye.get_stable_layout(self.__LAYOUT_STABLE_DEPTH)
        diffs = self.__ai_go.layout.compare_with(layout)

        same = False
        if len(diffs) == 0:
            same = True
        elif len(diffs) == 1:
            # Only one cell is different.
            cell_name, ai_color, scanned_color = diffs[0]
            if scanned_color == self.__WHITE:
                # And the scanned layout says: it's white color, because it's put by user, He runs so fast! :)
                same = True
        if same:
            print('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  Now please place your chess onto the board. ')
            self.__goto = self.at_state_user_play
            self.__mqtt.publish('gogame/smf/status','user_playing',retain=True)
            self.__mqtt.publish(topic="fishtank/switch/r4/command",payload="ON",retain=True)
        else:
            # more than one cells are different,  or the only one different cell is black color. 
            diffs = self.__ai_go.layout.compare_with(layout, do_print_out=True)
            time.sleep(10)

    def at_state_user_play(self):
        # check mark command, might be game over. 
        mark = self.__eye.get_stable_mark(self.__MARK_STABLE_DEPTH)
        
        if mark != 4:
            # Game over: 
            print(self.__BOLD + self.__FC_YELLOW + self.__BG_RED + 'Game Over!' + self.__FC_RESET)

            key = raw_input('Please confirm Game over by input "over"  ')
            if key == 'over':
                self.__ai_go.layout.clear()
                self.__ai_go.stop_game()
                self.__mqtt.publish('gogame/smf/status','game_over', retain=True)
                self.__goto = self.at_state_game_over
                return

        stable_layout = self.__eye.get_stable_layout(self.__LAYOUT_STABLE_DEPTH)
        do_print_diffs = False
        diffs = self.__ai_go.layout.compare_with(stable_layout)
        if len(diffs) == 0:
            # there is no use move
            pass
        elif len(diffs) == 1:
            # detected user move, and only one move, need to check color
            for cell_name, ai_color,detected_color in diffs:
                if ai_color==self.__BLANK and detected_color==self.__WHITE:
                    # detected cell is  White color. Means user put in a cell
                    print(self.__FC_PINK + 'detected: user has placed to cell: ' + cell_name)
                    # send command to PhonixGo
                    self.__ai_go.feed_user_move(cell_name)
                    self.__ai_go.layout.print_out()
                    self.__mqtt.publish('gogame/smf/status', 'computer_playing', retain=True)
                    self.__mqtt.publish(topic="fishtank/switch/r4/command", payload="OFF", retain=True)
                    self.__goto = self.at_state_scan_died_black
                    return
                else:
                    do_print_diffs = True
                    # self.__ai_go.layout.print_out()
                    # stable_layout.print_out()
        else:
            # more than one different, 
            # reason A: died area has not been entirely removed
            # reason B: detection is wrong
            # reason C: layout has benn disterbed by user. Like children playing bad with angry.
            do_print_diffs = True

        if do_print_diffs:
            diffs = self.__ai_go.layout.compare_with(stable_layout, do_print_out=True)
            print(self.__BG_RED + self.__FC_YELLOW + 'Too many different the between two layout.' + self.__FC_RESET)

        # print(diff_cell_name,my_color,detected_color)
        # logging.warn('diff_cell_name=%s, my_color=%d, detected_color=%d' %(diff_cell_name, my_color, detected_color))



    # def start(self):
    #     self.start_arm()
    #     self.start_mqtt()

    def at_demo_from_warehouse(self):
        layout = self.__eye.get_stable_layout(self.__MARK_STABLE_DEPTH)
        layout.print_out()
        cell = layout.get_first_cell(self.__BLANK)
        self.__arm.action_pickup_chess_from_warehouse()
        self.__arm.action_place_chess_to_a_cell(cell.name)
        # layout = self.__eye.get_stable_layout(self.__MARK_STABLE_DEPTH)
        # layout.print_out()
        self.__goto = self.at_state_game_over

    def at_demo_remove_to_trashbin_black(self):
        count = self.__remove_one_cell_to_trash(self.__BLACK)
        if count == 0:
            self.__remove_one_cell_to_trash(self.__WHITE)  
        self.__goto = self.at_state_game_over

    def at_demo_remove_to_trashbin_white(self):
        count = self.__remove_one_cell_to_trash(self.__WHITE)
        if count == 0:
            self.__remove_one_cell_to_trash(self.__BLACK)
        self.__goto = self.at_state_game_over

    def at_demo_mover(self):  # Must be no arguiment function for self.__goto
        do_vision_check = app_config.mainloop.at_demo_mover.do_vision_check
        layout = self.__eye.get_stable_layout(self.__LAYOUT_STABLE_DEPTH)
        layout.print_out()
        cell = layout.get_first_cell(self.__BLACK)
        if cell is not None:
            print('First black cell = %s' % cell.name)
            # self.__target_demo_layout.set_cell_value(cell.col_id, cell.row_id, self.__BLACK)
            id_black = cell.id
            self.__target_demo_layout.set_cell_value(cell.col_id, cell.row_id, self.__BLACK)
            cell = layout.get_first_cell(self.__WHITE)
            if cell is not None:
                self.__target_demo_layout.set_cell_value(cell.col_id, cell.row_id, self.__WHITE)
                print('First white cell = %s' % cell.name)
                # self.__target_demo_layout.set_cell_value(cell.col_id, cell.row_id, self.__WHITE)
                id_white = cell.id
                id = id_black
                if id_white < id_black:
                    id = id_white
                
                for i in range(id,359):
                    cell.from_id(i)
                    cell_color = layout.get_cell_color_col_row(cell.col_id, cell.row_id)
                    self.__arm.action_pickup_chess_from_a_cell(cell.name)
                    self.__target_demo_layout.set_cell_value(cell.col_id, cell.row_id, self.__BLANK)
                    cell.from_id(i+2)
                    self.__arm.action_place_chess_to_a_cell(cell.name,auto_park=do_vision_check)
                    self.__target_demo_layout.set_cell_value(cell.col_id, cell.row_id, cell_color)
                    if do_vision_check:
                        layout = self.__eye.get_stable_layout(self.__LAYOUT_STABLE_DEPTH)
                        diffs = layout.compare_with(self.__target_demo_layout, do_print_out = True)
                        if len(diffs) > 0:
                            cell_name, source_cell_color, target_cell_color = diffs[0]
                            app_config.robot_eye.layout_scanner.inspecting.cell_name = cell_name
                            key = raw_input ('Test failed! Please check')
                self.__arm.action_pickup_chess_from_a_cell('B19')
                self.__arm.action_place_chess_to_trash_bin(park_to_view_point=False)
                self.__target_demo_layout.set_cell_value_from_name('B19',self.__BLANK)
                self.__arm.action_pickup_chess_from_a_cell('A19')
                self.__arm.action_place_chess_to_trash_bin(park_to_view_point=True)
                self.__target_demo_layout.set_cell_value_from_name('A19',self.__BLANK)
                
        self.__goto = self.at_state_game_over

    def at_ending_demo(self):
        self.__goto = self.at_ending_demo

    def test_die_area_detector(self):
        layout = self.__eye.get_stable_layout(self.__MARK_STABLE_DEPTH)
        layout.print_out()
        
        die_helper = DiedAreaDetector()
        die_helper.start(layout.get_layout_array(),self.__BLACK)


    def spin_once(self):
        self.__last_image = self.__eye.take_picture()
        if config.publish_mqtt:
            g_mqtt.publish_cv_image('gobot/head/eye/origin',self.__last_image)

        command_image = 1
        chessboard_image = 1
        warehouse_image = 1

        last_function = self.__goto
        self.__goto()
        if last_function != self.__goto:
            print(self.__BG_BLUE + self.__FC_YELLOW)
            print(self.__goto.__name__)
            print(self.__FC_RESET)


        # oringin_image = self.__eye.take_picture()
        # command_image = self.__command_finder.auto_perspect(oringin_image)
        # command = self.__command_scanner.get_command(command_image)
        # if command: 
        #     self.user_playing()
                
        



if __name__ == '__main__':
    config.publish_mqtt = True
    if config.publish_mqtt:
        g_mqtt.connect_to_broker('123457','voicevon.vicp.io',1883,'von','von1970')

    myrobot = GobotHead()
    while True:
        myrobot.spin_once()


    while True:
        menu = []
        menu.append('***********************************************************')
        menu.append('* 1. (TODO) List cameras, You can see the IDs.            *')
        menu.append('* 2. Capture image from camera (id = 0)                   *')
        menu.append('* 3.                                           *')
        menu.append('* 4. get_stable_layout()                                  *')
        menu.append('* 5. Scan instruction marks(mode=?)                       *')
        menu.append('* 6. get_stable_mark()                                    *')
        menu.append('* 7. Died_area_scanner                                    *')
        menu.append('***********************************************************')

