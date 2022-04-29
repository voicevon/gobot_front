
# from vision.robot_eye_pi_camera import MonoEyePiCamera
# from vision.robot_eye_usb_camera import MonoEyeUsbCamera
# from vision.robot_eye_emulator import MonoEyeEmulator
from gc import garbage
from vision.robot_eye_factory import RobotEye_Factory, RobotEye_Product

from gobot_vision.gobot_vision import GobotVision
from gogame.chessboard_cell import ChessboardCell, StoneColor
from gogame.chessboard import ChessboardLayout
from gogame.died_area_scanner import DiedAreaScanner
# from controller import Controller
from gobot_ai_client import GoGameAiClient
from config.config import Config

from von.mqtt_helper import g_mqtt, MQTT_ConnectionConfig
from Pylib.rabbit_mq_helper import g_amq, AMQ_ConnectionConfig
from Pylib.rabbitmq_mqtt_sync import SyncerFactory
from Pylib.image_logger import ImageLogger,ImageLoggerToWhere
from Pylib.message_logger import MessageLoggerToWhere,MessageLogger


import logging
import time

from von.terminal_font import TerminalFont  # pip3 install VonPylib
from gogame.human_level_gobot_arm import ArmMap, HumanLevelGobotArm
from gogame.human_level_gobot_house import HumanLevelGobotHouse
from vision.robot_eye_base import MonoEyeBase

def Init_Global():
        # logging.basicConfig(level=logging.DEBUG)
        logging.basicConfig(level=logging.CRITICAL)

        # config_mqtt = MQTT_ConnectionConfig()
        # config_mqtt.uid = 'agent'
        # config_mqtt.password = 'agent'
        # g_mqtt.connect_to_broker(config_mqtt)

        config_rabbit = AMQ_ConnectionConfig()
        config_rabbit.uid = 'agent'
        config_rabbit.password = 'agent'
        g_amq.ConnectToRabbitMq(config_rabbit)

def MakeEye(eye_type:RobotEye_Product) -> MonoEyeBase:
    eye = RobotEye_Factory.CreateMonoEye(eye_type)
    
    if eye_type == RobotEye_Product.PaspberryPiCamera:
        ImageLogger.to_where = ImageLoggerToWhere.TO_MQTT
        ImageLogger.connect_to_mqtt_broker()
        MessageLogger.to_where = MessageLoggerToWhere.TO_MQTT

    elif eye_type == RobotEye_Product.CameraEmulator:
        ImageLogger.to_where = ImageLoggerToWhere.TO_MQTT
        MessageLogger.to_where = MessageLoggerToWhere.TO_SCREEN

    elif eye_type == RobotEye_Product.UsbCamera:
        ImageLogger.to_where = ImageLoggerToWhere.TO_SCREEN
        MessageLogger.to_where = MessageLoggerToWhere.TO_SCREEN
    return eye

Init_Global()
g_eye = MakeEye(eye_type=RobotEye_Product.UsbCamera)
g_arm = HumanLevelGobotArm()
g_house = HumanLevelGobotHouse()
g_vision = GobotVision()
g_ai = GoGameAiClient()
print("[Info] Global vars init is done........\n\n\n\n")

class GobotHead_Demo():
    def __init__(self):
        self.__LAYOUT_STABLE_DEPTH = 3

    def at_demo_from_warehouse(self, origin_image) -> bool:
        global g_vision
        global g_arm
        # layout = self.__eye.get_stable_layout(self.__MARK_STABLE_DEPTH)
        layout,depth = g_vision.get_chessboard_layout(origin_image)
        if layout is None:
            return False

        layout.print_out()
        cell = layout.get_first_cell(StoneColor.BLANK)
        g_arm.Pickup_Place(from_where='house', to_where=cell.name)
        return True

    def mover(self, origin_image):
        global g_vision
        do_vision_check = Config.mainloop.at_demo_mover.do_vision_check
        layout, stable_depth = g_vision.get_chessboard_layout(origin_image)
        if stable_depth <=0:
            print('Can NOT detect chessboard image ')
            return
        layout.print_out()
        print('Stable depth = ',stable_depth)
        if stable_depth < self.__LAYOUT_STABLE_DEPTH:
            return 

        # Got dedicate stable layout
        layout.print_out()
        cell = layout.get_first_cell(StoneColor.BLACK)
        if cell is not None:
            print('First black cell = %s' % cell.name)
            # self.__target_demo_layout.set_cell_value(cell.col_id, cell.row_id, StoneColor.BLACK)
            id_black = cell.id
            self.__target_demo_layout.set_cell_value(cell.col_id, cell.row_id, StoneColor.BLACK)
            cell = layout.get_first_cell(StoneColor.WHITE)
            if cell is not None:
                self.__target_demo_layout.set_cell_value(cell.col_id, cell.row_id, StoneColor.WHITE)
                print('First white cell = %s' % cell.name)
                # self.__target_demo_layout.set_cell_value(cell.col_id, cell.row_id, StoneColor.WHITE)
                id_white = cell.id
                id = id_black
                if id_white < id_black:
                    id = id_white
                
                for i in range(id,359):
                    cell.from_id(i)
                    cell_color = layout.get_cell_color_col_row(cell.col_id, cell.row_id)
                    self.__controller.action_pickup_stone_from_cell(cell.name)
                    self.__target_demo_layout.set_cell_value(cell.col_id, cell.row_id, StoneColor.BLANK)
                    cell.from_id(i+2)
                    self.__controller.action_place_stone_to_cell(cell.name,auto_park=do_vision_check)
                    self.__target_demo_layout.set_cell_value(cell.col_id, cell.row_id, cell_color)
                    if do_vision_check:
                        layout = self.__eye.get_stable_layout(self.__LAYOUT_STABLE_DEPTH)
                        diffs = layout.compare_with(self.__target_demo_layout, do_print_out = True)
                        if len(diffs) > 0:
                            cell_name, source_cell_color, target_cell_color = diffs[0]
                            Config.robot_eye.layout_scanner.inspecting.cell_name = cell_name
                            key = raw_input ('Test failed! Please check')
                # self.__controller.action_pickup_stone_from_cell('B19')
                # self.__controller.action_place_stone_to_trash_bin(park_to_view_point=False)
                self.__arm.Pickup_Place(from_where='B19', to_where='trash_bin')
                self.__target_demo_layout.set_cell_value_from_name('B19',StoneColor.BLANK)
                # self.__controller.action_pickup_stone_from_cell('A19')
                # self.__controller.action_place_stone_to_trash_bin(park_to_view_point=True)
                self.__arm.Pickup_Place(from_where="A19", to_where='trash_bin')
                self.__target_demo_layout.set_cell_value_from_name('A19',StoneColor.BLANK)

            pass

    def at_demo_remove_to_trashbin_white(self):
        return
        count = self.__remove_one_cell_to_trash(StoneColor.WHITE)
        if count == 0:
            self.__remove_one_cell_to_trash(StoneColor.BLACK)
        # self.__goto = self.at_state_game_over

    def at_demo_remove_to_trashbin_black(self):
        return
        count = self.__remove_one_cell_to_trash(StoneColor.BLACK)
        if count == 0:
            self.__remove_one_cell_to_trash(StoneColor.WHITE)  
        self.__goto = self.at_state_game_over
# class GobotPlayer():
#     def __init__(self) -> None:
#         pass



'''
.                      |<------------------------------------------------------<
.                      |                                                       ^
.   gameover  -->  user_play --> draw_white --> computer_play --> draw_black --|
.       |        |---------> gameover
.       | 
.       |->  clear_board -----> demo_over  --> game_over                      
.       |->  demo_mover  ---------->|
.       |->  demo_place  ---------->|
.             
'''

class GobotHead():
    '''
    GobotHead is the main and starter of this application.
    GobotHead is composed by:
    1. eye, it read camera
    2. vision, will recognize some components.
    *  chessboard_vision, can get ChessboardLayout.
    *  command_vision, can get mark on command bar.
    *  house_vision, cam get park_point is empty or not.
    3. talker_xxx, will communicate with out side.
    3.1 talker_ai, can talk to ai-go-game-player
    3.2 talker_mqtt, can talk to mqtt broker
        3.2.1  subscribe topics for parameter, functions
        3.2.2  public images, states, etc for debugging.
    3.3 talker_arm, can talk to chessboard robot arm.
    3.4 talker_house, can talk to house robot arm.
    '''
    def __init__(self, serial_id:int):
        self.Serial_id = serial_id
        # self.body = GobotBody(eye_type=eye_type)

        # self.arm = HumanLevelGobotArm()
        # self.house = HumanLevelGobotHouse()

        self.demor = GobotHead_Demo()
        self.__died_area_scanner = DiedAreaScanner()
        self.__goto = self.at_state_game_over
        self.__target_demo_layout = ChessboardLayout('Demo Layout')
        self.__last_detected_layout = ChessboardLayout('Last_detected')

        self.__InitOthers()
        print("[Info] GobotHead::__init__()  is done.")


    def __InitOthers(self):
        logging.info("[Info] GobotHead Start init objects......")

        self.__FC_YELLOW = TerminalFont.Color.Fore.yellow
        self.__BG_BLUE = TerminalFont.Color.Background.blue
        self.__BG_RED = TerminalFont.Color.Background.red
        self.__FC_GREEN = TerminalFont.Color.Fore.green
        self.__FC_RESET = TerminalFont.Color.Control.reset
        self.__FC_PINK = TerminalFont.Color.Fore.pink
        self.__BOLD = TerminalFont.Color.Control.bold
        self.__MARK_STABLE_DEPTH = 5
        self.__LAYOUT_STABLE_DEPTH = 2

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

    # def get_stable_mark(self,min_stable_depth):
    #     stable_depth = 0
    #     while stable_depth < min_stable_depth:
    #         ret, img = self.__capture_device.read()
    #         if ret:
    #             mark_index, stable_depth = self.__mark_scanner.detect_mark(img, min_stable_depth)
    #     return mark_index
        
    
    def __remove_one_cell_to_trash(self, color):
        '''
        return: how many stones have benn removed
            0 = zero chess has been removed
            1 = one chess has been removed
        '''
        (the_layout, stable_depth) = self.body.vision.get_chessboard_layout(self.__last_image)
        if the_layout is None:
            print("[Warn] GobotHead::__remove_one_cell_to_trash()   the_layout is None")
            return
        the_layout.print_out()
        from_cell = the_layout.get_first_cell(color)
        if from_cell is not None:
            logging.info(self.__FC_GREEN + '[INFO]: GoManager.__remove_one_cell_to_trash() ' + from_cell.name + self.__FC_RESET)
            self.body.arm.action_pickup_stone_from_cell(from_cell)
            self.body.arm.action_place_stone_to_trash_bin()
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
        area = died_area_helper.start(self.__ai.layout.get_layout_array(), target_color_code)
        for col in range(0,19):
            for row in range(0,19):
                if area[col][row] == 0:
                    cell = ChessboardCell()
                    cell.from_col_row_id(col,row)
                    # self.__controller.action_pickup_stone_from_cell(cell.name)
                    # self.__controller.action_place_stone_to_trash_bin()
                    self.__ai.layout.play_col_row(col,row,StoneColor.BLANK)

    def at_state_demo_over(self):
        global g_vision
        print("Demo is over. Please give me a new command via placing CommandStone,  last_command is  ", self.last_command)
        new_command = g_vision.get_command_index(self.__last_image)
        if new_command != self.last_command:
            print("Got new command, new command is  ", new_command )
            self.__goto = self.at_state_game_over
        time.sleep(5)

    def at_state_game_over(self):
        '''
        scan the marks, to run markable command
        '''
        global g_vision
        command = g_vision.get_command_index(self.__last_image)
        # print('Commander id = %d', command)

        if command == 0:
            if self.demor.at_demo_from_warehouse(self.__last_image):
                self.__goto = self.at_state_demo_over

        if command == 1:
            self.demor.mover(self.__last_image)
            self.__goto = self.at_state_demo_over

        if command == 2:
            self.demor.at_demo_remove_to_trashbin_black()
            self.__goto = self.at_state_demo_over
  
        elif command == 3:
            self.demor.at_demo_remove_to_trashbin_white()
            self.__goto = self.at_state_demo_over

        elif command == 4:
            self.__goto = self.at_state_begin
            MessageLogger.Output('gobot/smf/current','play')
        else:
            logging.warning(self.__FC_YELLOW + '[Warning]: GoManger.at_begining()  scanned command=%d' %command)

        self.last_command = command

    def at_state_begin(self):
        '''
        scan the marks, to run markable command
        '''
        global g_vision
        global g_ai

        command = g_vision.get_command_index(self.__last_image)
        if command == 4:
            g_ai.start_new_game()
            g_vision.init_chessboard_layout()
            MessageLogger.Output('gobot/smf/current', 'user_playing')

            self.__goto = self.at_state_user_play
        else:
            print(self.__FC_YELLOW + '[Warning]: GobotHead.at_state_begining()  scanned command=%d' %command)
            self.__goto = self.at_state_game_over

    def at_state_user_play(self):
        '''
        * User is always play BLACK stone.
        * check mark command, might be game over.
        ''' 
        global g_vision
        global g_ai

        mark = g_vision.get_command_index(self.__last_image)
        
        if (mark != 4) and (mark !=-1):
            # Game over: 
            logging.info(self.__BOLD + self.__FC_YELLOW + self.__BG_RED + 'Game Over!' + self.__FC_RESET)

            key = input('Please confirm Game over by input "over"  ')
            if key == 'over':
                self.__ai.layout.clear()
                self.__ai.stop_game()
                MessageLogger.Output('gobot/smf/current','game_over')
                self.__goto = self.at_state_game_over
                return

        stable_layout, stable_depth = g_vision.get_chessboard_layout(self.__last_image)
        if stable_depth < 3: return
        
        MessageLogger.Output("user_play_Stable_depth", stable_depth)
        update = self.__last_detected_layout.compare_with(stable_layout, do_print_out=False)
        if len(update) == 0:
        # if self.__last_detected_layout == stable_layout:
            return
        # self.__last_detected_layout.compare_with(stable_layout, do_print_out=True)
        self.__last_detected_layout = stable_layout  # should be a copy

        do_print_diffs = False
        diffs = g_ai.layout.compare_with(stable_layout,do_print_out=True)
        diffs_len = len(diffs)
        # MessageLogger.Output("Gobot_Head().at_state_user_play()", diffs_len)
        # MessageLogger.Output("Gobot_Head().at_state_user_play()", diffs)
        if diffs_len == 0:
            # there is no user move
            pass
        elif diffs_len == 1:
            # detected one stone is placed, and only one stone is placed, need to check color
            for cell_name, ai_color, detected_color in diffs:
                if ai_color==StoneColor.BLANK and detected_color==StoneColor.BLACK:
                    # detected the placed stone is black color. Means user put a stone onto a cell
                    print(self.__FC_PINK + 'detected: user has placed stone onto cell: ' + cell_name)
                    # send command to PhonixGo
                    g_ai.feed_user_move(cell_name)
                    g_ai.layout.print_out()
                    MessageLogger.Output('gogame/smf/status', 'computer_playing')
                    MessageLogger.Output("fishtank/switch/r4/command", "OFF")
                    self.__goto = self.at_state_scan_died_white
                    return
                else:
                    do_print_diffs = True
                    # self.__ai.layout.print_out()
                    # stable_layout.print_out()
        else:
            # more than one different, 
            # reason A: died area has not been entirely removed
            # reason B: vision recognization is wrong.
            # reason C: layout has benn disterbed by user. Like children playing bad with angry.
            do_print_diffs = True

        if do_print_diffs:
            diffs = g_ai.layout.compare_with(stable_layout, do_print_out=True)
            print(self.__BG_RED + self.__FC_YELLOW + 'Too many different the between two layout.' +  str(len(diffs)) + self.__FC_RESET)

    def at_state_computer_play(self):
        '''
        Computer is always play White
        '''
        global g_arm
        global g_ai

        g_ai.get_final_score()
        # get command from PhonixGo
        cell_name = g_ai.get_ai_move()
        if cell_name is not None:
            # some time the ai_player will return a 'resign' as a cell name.
            logging.info(self.__FC_PINK + 'AI step: place black at: %s' %cell_name + self.__FC_RESET)
            # robot arm play a chess, The instruction is from AI.
            # self.__controller.action_pickup_stone_from_warehouse()
            # self.__controller.action_place_stone_to_cell(cell_name=cell_name)
            from_where = ArmMap.VENDER
            cell = ChessboardCell()
            cell.from_name(cell_name)
            to_where = ArmMap.CELLS(cell)

            g_arm.Pickup_Place(from_where, to_where=to_where)
            g_ai.layout.play(cell_name, StoneColor.WHITE)
            g_ai.layout.print_out()

        self.__goto = self.at_state_scan_died_black

    def at_state_withdraw_white(self):
        cell = self.__died_area_scanner.get_first_died_cell()
        if cell is None:
            # There is no died area to be removed
            self.__goto = self.at_state_computer_play
        else:
            # only remove one cell inside the died area.
            # will go on to remove other cells on the next invoking
            # self.__controller.action_pickup_stone_from_cell(cell.name)
            # self.__controller.action_place_stone_to_trash_bin()
            self.__arm.Pickup_Place(from_where=cell.name, to_where='trash_bin')
            self.__ai.layout.play(cell.name, StoneColor.BLANK)
            self.__died_area_scanner.died_cell_removed_first_one()

    def at_state_withdraw_black(self):
        global g_arm

        cell = self.__died_area_scanner.get_first_died_cell()
        if cell is None:
            # There is no died area to be removed
            self.__goto = self.at_state_user_play
        else:
            # only remove one cell of the died area.
            # will go on to remove other cells on the next invoking
            g_arm.action_pickup_stone_from_cell(cell.name)
            g_arm.action_place_stone_to_trash_bin()
            # self.__arm.Pickup_Place(from_where=cell.name, to_where='trash_bin')
            self.__ai.layout.play(cell.name, StoneColor.BLANK)
            self.__died_area_scanner.died_cell_removed_first_one()

    def at_state_scan_died_white(self):
        '''
        * This is involked after user playing.
        '''
        global g_ai
        self.__died_area_scanner.set_layout_array(g_ai.layout.get_layout_array())
        count = self.__died_area_scanner.start_scan(StoneColor.WHITE)
        if count > 0:
            self.__died_area_scanner.print_out_died_area()

        self.__goto = self.at_state_withdraw_white

    def at_state_scan_died_black(self):
        global g_ai
        self.__died_area_scanner.set_layout_array(g_ai.layout.get_layout_array())
        count = self.__died_area_scanner.start_scan(StoneColor.BLACK)
        if count > 0:
            self.__died_area_scanner.print_out_died_area()
        self.__goto = self.at_state_withdraw_black

    def at_state_compare_layout_black_old(self):
        '''
        * Doing withdraw black stones.
        '''
        global g_ai
        layout = self.__eye.get_stable_layout(self.__LAYOUT_STABLE_DEPTH)
        diffs = g_ai.layout.compare_with(layout)
        if len(diffs) == 0:
            # Withdraw the black is finished.
            self.__goto = self.at_state_computer_play
        else: 
            # Withdraw is not finished.
            diffs = g_ai.layout.compare_with(layout, do_print_out=True)
            time.sleep(10)

    def at_state_compare_layout_black(self):
        '''
        * Doing withdraw black stones.
        '''
        global g_ai

        layout, stable_depth = self.__vision.get_chessboard_layout(self.__last_image)
        diffs = g_ai.layout.compare_with(layout)

        same = False
        if len(diffs) == 0:
            same = True
        elif len(diffs) == 1:
            # Only one cell is different.
            cell_name, ai_color, scanned_color = diffs[0]
            if scanned_color == StoneColor.WHITE:
                # And the scanned layout says: it's white color, because it's put by user, He runs so fast! :)
                same = True
        if same:
            logging.info('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  Now please place your Stone onto the board. ')
            self.__goto = self.at_state_user_play
            MessageLogger.Output('gobot/smf/current','user_playing')
            MessageLogger.Output("fishtank/switch/r4/command","ON")
        else:
            # more than one cells are different,  or the only one different cell is black color. 
            diffs = g_ai.layout.compare_with(layout, do_print_out=True)
            time.sleep(10)

    def SpinOnce(self):
        global g_eye
        # self.SyncHelper.SpinOnce()

        # self.__last_image = self.__eye.take_picture(do_undistort=True)
        self.__last_image = g_eye.take_picture(do_undistort=False)
        ImageLogger.Output("gobot_x2134_eye_origin", self.__last_image, to_where=ImageLoggerToWhere.TO_AMQ)
        # return

        command_image = 1
        chessboard_image = 1
        warehouse_image = 1

        #self.__vision.get_warehouse_plate(self.__last_image)
        last_function = self.__goto
        # print(self.__goto)
        self.__goto()
        if last_function != self.__goto:
            print(self.__BG_BLUE + self.__FC_YELLOW)
            print(self.__goto.__name__)
            print(self.__FC_RESET)

    def test_user_playing(self):
        self.__ai.send("move c4 B")

    def test(self):
        self.__controller.home_single_arm(4)
        for i in range(1,20,1):
            self.__controller.spin_once()
            # time.sleep(1)
        #time.sleep(20)
        return
        self.__controller.action_pickup_stone_from_warehouse()
        self.__controller.action_place_stone_to_cell('Q4')
        self.__controller.action_place_stone_to_trash_bin()
        self.__controller.action_park()


        

if __name__ == '__main__':

    Init_Global()

    # robot_eye = RobotEye_Product.PaspberryPiCamera
    # robot_eye_type = RobotEye_Product.UsbCamera
    # robot_eye_type= RobotEye_Product.CameraEmulator

    myrobot = GobotHead(2134)
    # myrobot = GobotHead(RobotEye_Product.PaspberryPiCamera)
    # myrobot.house.demo()
    i = 0
    while True:
        myrobot.SpinOnce()
        # print(i)
        i += 1



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

