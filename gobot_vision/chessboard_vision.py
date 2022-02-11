from config.image_logger import ImageLogger
from gobot_vision.cell_scanner import CellScanner
from gogame.chessboard import ChessboardLayout
from gogame.chessboard_cell import ChessboardCell, Stone
from config.config import Config as app_config
import cv2
import numpy
import sys
sys.path.append('/home/pi/pylib')
from von.terminal_font import TerminalFont
# from von.mqtt_helper import g_mqtt

# BLANK = 0
# WHITE_STONE = 1
# BLACK_STONE = 2


class config_4_aruco_marks:
    name='CHESSBOARD_ARUCO_CONFIG'
    top_right_id =  13
    bottom_right_id = 34
    bottom_left_id = 21
    top_left_id = 15

    perspective_width = 428 -6   #effect scale, greater number cause zoom in. 
    perspective_height = 428 + 5  # effect scale
    top_extended = -7
    right_extended = 5   # effect view range
    bottom_extended = 5   # effect view range
    left_extended = 0

    crop_x0 = 0   # effect the left edge
    crop_y0 = 0   # effect the top edge.
    crop_width = 428   # Don't modify this value
    crop_height = 428  # Don't modify this value.
    


class ChessboardVision():
    def __init__(self):
        '''
        Main content:
            1. Know Aruco config of chessboard
            2. Generate layout from chessboard_image
            3. Never touch with origin_image! 
        '''
        self.layout = self.create_blank_layout()

        self.__detected_layout = ChessboardLayout('Detected layout')

        # history
        self.__history = []
        self.__history_length = 0 
        # self.__diffs = []

        self.__BLANK = Stone.BLANK
        self.__BLACK = Stone.BLACK
        self.__WHITE = Stone.WHITE
        self.__ROWS = 19
        self.__COLS = 19

        self.__SPACE_X = 22
        self.__SPACE_Y = 22
        self.__VIEW_RANGE = 1.6

        self.__inspect_cell =  ChessboardCell()
        # self.__inspect_cell.from_name(app_config.robot_eye.layout_scanner.inspecting.cell_name)

        self.__FC_YELLOW = TerminalFont.Color.Fore.yellow
        self._BG_GREEN = TerminalFont.Color.Background.green
        self.__FC_RESET = TerminalFont.Color.Control.reset

        self.Min_BlackColor = numpy.array([0, 0, 0])  # 要识别黑子颜色的下限
        self.Max_BlackColor = numpy.array([180, 255, 80])  # 要识别黑子的颜色的上限
        self.Min_WhiteColor = numpy.array([0, 0, 200])  # 要识别白子颜色的下限
        self.Max_WhiteColor = numpy.array([180, 100, 255])  # 要识别白子的颜色的上限


    def create_blank_layout(self):
        layout = [([0] * 19) for i in range(19)]
        return layout



    def get_4_aruco_marks_config(self):
        return config_4_aruco_marks

 
    def __append_to_history(self, layout) -> int:
        '''
        return:
            stable_depth, minimum number is 1.
        '''
        while len(self.__history) > self.__history_length:
            del self.__history[0]
        self.__history.append(layout)

        # update stable_depth
        stable_depth = 1
        if len(self.__history) > 2:
            for i in range(len(self.__history)-1):
                diffs = self.__history[-1].compare_with(self.__history[i]) 
                if len(diffs) == 0:
                    # no different
                    stable_depth += 1
        return stable_depth

    def get_layout_from_image(self, chessboard_image):
        '''
        Descrepted!
        '''
        return self.start_scan(chessboard_image)

        
    def start_scan(self, img_board, history_length=3, show_processing_image=True, pause_second=1):
        '''
        Try to get layout from image.
        return A:
            -1,-1: not detected any layout
        return B:
            the lastest layout in history,
            stable_depth
        '''
        self.__history_length = history_length
        detected_layout = ChessboardLayout('Detected layout')
       # detected_layout.print_out()
        board_gray = cv2.cvtColor(img_board, cv2.COLOR_BGR2GRAY)
        board_brightness = numpy.mean(board_gray)
        # print('board_brightness()= %d' %board_brightness)

        if app_config.publish_image_board_gridline.value:
            lined_image = img_board.copy()
            line_color = (255,255,0)
            pen_width = 1
            for row in range(0,self.__ROWS):
                y = int(row * self.__SPACE_Y + self.__SPACE_Y / 2)
                x1 = 0
                x2 = int(19 * self.__SPACE_X)
                cv2.line(lined_image, (x1,y),(x2,y), line_color, pen_width)
            for col in range(0,self.__COLS):
                x = int(col * self.__SPACE_X + self.__SPACE_X / 2)
                y1 = 0
                y2 = 19 * self.__SPACE_Y
                cv2.line(lined_image, (x,y1),(x,y2), line_color, pen_width)
            # g_mqtt.publish_cv_image('gobot/image/chessboard/gridline',lined_image)     
            ImageLogger.Output('gobot/image/chessboard/gridline',lined_image)     

        cell_scanner = CellScanner(board_brightness)
        # Split board_image to 361 samll images. detect circle one by one
        for col in range(0,self.__COLS):
            for row in range(0,self.__ROWS):
                # crop to small image, around cell center
                # (x1,y1) is the topleft point of the cell
                x1 = self.__SPACE_X * col 
                y1 = self.__SPACE_Y * row 
                # (x2,y2) is the bottomright point of the cell
                x2 = x1 + int(self.__SPACE_X * self.__VIEW_RANGE)
                y2 = y1 + int(self.__SPACE_Y * self.__VIEW_RANGE)
                    
                cell_img_big = img_board[y1:y2, x1:x2]

                shrink_size = 6
                x1 = self.__SPACE_X * col + shrink_size
                y1 = self.__SPACE_Y * row + shrink_size
                x2 = x1 + int(self.__SPACE_X * self.__VIEW_RANGE - 2 * shrink_size)
                y2 = y1 + int(self.__SPACE_Y * self.__VIEW_RANGE - 2 * shrink_size)
                cell_img_small = img_board[y1:y2, x1:x2]

                is_inspected_cell = False
                self.__inspect_cell.from_name(app_config.robot_eye.layout_scanner.inspecting.cell_name)
                if (col == 18 - self.__inspect_cell.col_id) and (18- row == self.__inspect_cell.row_id):
                    # cv2.imshow('bbbb',cell_img_big)
                    # cv2.imshow('ssss',cell_img_small)
                    is_inspected_cell = True
                # color = cell_scanner.scan(cell_img,is_inspected_cell)
                color = cell_scanner.scan_white(cell_img_big, is_inspected_cell)
                detected_layout.play_col_row(col_id=18-col, row_id=18-row, color_code=color)
                if color != self.__WHITE:
                    color = cell_scanner.scan_black(cell_img_small, is_inspected_cell)
                    detected_layout.play_col_row(col_id=18-col, row_id=18-row, color_code=color)
        stable_depth = self.__append_to_history(detected_layout)
        # if app_config.robot_eye.layout_scanner.show_scan_image:
        #     self.__show_debug(img_board,stable_depth)

        #self.__history[-1].print_out()

        # return self.__history[-1], stable_depth
        return detected_layout, stable_depth


    def __show_debug(self, img_board,stable_depth):
        copy = img_board.copy()
        cv2.putText(copy, 'Depth= ' + str(stable_depth),(10,50),cv2.FONT_HERSHEY_SIMPLEX,1,(0,255,0),1)
        if len(self.__history) > 2:
            diffs = self.__history[-1].compare_with(self.__history[-2])
            text = ''
            for name,color_a, color_b in diffs:
                text += '' + name + ','
               # draw a red circle on each different cells
                diff_cell = ChessboardCell()
                diff_cell.from_name(name)
                x,y = diff_cell.to_camera__board_xy()
                cv2.circle(copy, (x,y), 16, (0,0,255), 2)
            # draw a blue circle on last_moving cell
            cell = ChessboardCell()
            cell_name = app_config.current_game.lastest_move_cell_name
            if cell_name is not None:
                cell.from_name(cell_name)
                x,y = cell.to_camera__board_xy()
                cv2.circle(copy, (x,y),16, (255,0,0), 3)
            cv2.putText(copy, 'diffs= ' + text, (10,100), cv2.FONT_HERSHEY_SIMPLEX, 1, (0,255,0), 1)
            # cv2.imshow('layout_scanner', copy)
            
            #is_success, img_encode = cv2.imencode(".jpg", copy)
            #if is_success:
            #    img_pub = img_encode.tobytes()
            #    app_config.server.mqtt.client.publish(topic='gogame/eye/layout_scanner/chess_board', payload=img_pub)
            cv2.waitKey(1)
