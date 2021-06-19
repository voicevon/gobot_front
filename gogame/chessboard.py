import sys
sys.path.append('/home/pi/gobot_front')

from vision.grid_layout import GridLayout
from gogame.chessboard_cell import ChessboardCell, Stone
import sys
sys.path.append('/home/pi/pylib')
from terminal_font import TerminalFont
from mqtt_helper import g_mqtt


class ChessboardLayout(GridLayout):

    def __init__(self,name):
        '''
        We can play go game with ChessboardLayout() ?
            1. Place/Remove a stone onto a cell.
            2. Compare two layout, return the differencial.
            3. Calculate died area.

        When we deal with a batch of ChessboardLayouts,
            A chessboard.name does help.
        '''
        self.name = name

        self._ROWS = 19
        self._COLS = 19
        
        self._BLANK = Stone.Blank
        self._BLACK = Stone.Black
        self._WHITE = Stone.White


        self._FC_YELLOW = TerminalFont.Color.Fore.yellow
        self._BG_RED = TerminalFont.Color.Background.red
        self._FC_RESET = TerminalFont.Color.Control.reset

        self._layout_array = [([0] * self._COLS) for i in range(self._ROWS)]

    def rename_to(self, new_name):
        self.name = new_name

    def create_blank_layout(self):
        '''
        Note: return nothing
        '''
        self._layout_array = [([0] * self._COLS) for i in range(self._ROWS)]

    def clear(self):
        for i in range(19):
            for j in range(19):
                self._layout_array[i][j] = 0

    def get_layout_array(self):
        return self._layout_array



    def play_col_row(self, col_id, row_id, color_code):
        cell = ChessboardCell()
        cell.from_col_row_id(col_id=col_id, row_id=row_id)
        self.play(cell.name, color_code)

    def play(self, cell_name, stone_color):
        # print('[Info] ChessBoard.play(cell_name=%s,color=%s)' %(cell_name,color))
        cell = ChessboardCell()
        cell.from_name(cell_name)
        # value = self.__BLANK
        # if color =='Black':
        #     value = self._BLACK
        # elif color == 'White':
        #     value = self._WHITE
        # else:
        #     logging.info('ChessBoard.play(cell_name=%s,color=%s)' %(cell_name,color))
        self._layout_array[cell.col_id][cell.row_id] = stone_color


    def get_cell_color(self, cell):
        col = cell.col
        row = cell.row
        return self.get_cell_color_col_row(col,row)

    def get_cell_color_col_row(self, col_id, row_id):
        return self._layout_array[col_id][row_id]
    
    def get_first_cell(self, target_stone_color):
        '''
        return:
            (x,y) is the target position
            (-1,-1) means not found!
        '''
        cell = ChessboardCell()
        for row_id in range(0, self._ROWS):
            for col_id in range(0, self._COLS):
                if self._layout_array[col_id][row_id] == target_stone_color:
                    cell.from_col_row_id(col_id, row_id)
                    return cell
        return None

    def set_cell_value_from_name(self, cell_name, new_value):
        cell = ChessboardCell()
        cell.from_name(cell_name)
        self.set_cell_value(cell.col_id, cell.row_id, new_value)

    def set_cell_value(self, col_id, row_id, new_value):
        self._layout_array[col_id][row_id] = new_value
        
    def compare_with(self, target_layout, do_print_out=False):
        '''
        return: 
            total_diff_cells: the number of different cells.
            last_cell_name: first cell_name that different, might be None
            my_color: color of last_cell
            target_color: color of last_cell
        '''
        diffs = []
        cell = ChessboardCell()
        my_cell_color = target_cell_color = self.__BLANK

        int_to_char = {self.__BLANK:'. ',self._BLACK:'X ', self._WHITE:'O '}
        if do_print_out:
            title = self._FC_RESET + '       ' 
            title += self._BG_RED + self._FC_YELLOW + self.name 
            title += self._FC_RESET + '                                         '
            title += self._BG_RED + self._FC_YELLOW + target_layout.name + self._FC_RESET
            print(title)

            # print column name on table head
            header = '  '
            for col_id in range (self._COLS,-1,-1):
                cell.from_col_row_id(col_id=col_id, row_id=1)
                header += cell.col_letter + ' '
            header += '  ' + header
            print(self._FC_YELLOW + header)

        # print layout row by row
        for row_id in range(self._ROWS-1, -1, -1):
            rowNum = row_id + 1
            my_col_string = target_col_string = ''
            for col_id in range(self._COLS-1, -1, -1):
                print_color = self._FC_RESET
                target_cell_color = target_layout.get_cell_color_col_row(col_id=col_id, row_id=row_id)
                if (self._layout_array[col_id][row_id] != target_cell_color):
                    #color is different
                    print_color = self._BG_RED + self._FC_YELLOW  
                    cell.from_col_row_id(col_id=col_id, row_id=row_id)
                    my_cell_color = self._layout_array[col_id][row_id]
                    diffs.append((cell.name, my_cell_color, target_cell_color))
                
                if do_print_out:
                    my_col_string += print_color + int_to_char[self._layout_array[col_id][row_id]] + self._FC_RESET
                    k = target_layout.get_cell_color_col_row(col_id, row_id)
                    target_col_string += print_color + int_to_char[k] + self._FC_RESET
            if do_print_out:
                row_string = self._FC_YELLOW + "%02d" % rowNum
                col_string = row_string +' '+ my_col_string + ' ' + row_string + '  ' + target_col_string + ' ' + row_string
                print(col_string )
        if do_print_out:
            print(self._FC_YELLOW + header)
            print(self._BG_RED + self._FC_YELLOW + '%s' %diffs + self._FC_RESET)
        return diffs
        # return total,cell.name ,my_cell_color, target_cell_color




        # for row in range(0, 19):
        #     for col in range(0, 19):
        #         target_cell_color = target_layout.get_cell_color_col_row(col_id=col,row_id=row)
        #         if (self._layout_array[col][row] != target_cell_color):
        #             cell.from_col_row_id(col_id=col, row_id=row)
        #             my_cell_color = self._layout_array[col][row]

        #             # print('[INFO]: ChessboardMap.compare_cell_map() diff at: %s' %cell.name)
        # return total,cell.name ,my_cell_color, target_cell_color

        # # self.print_cell_map()
        # # target_layout.print_cell_map()
        # # TODO: What if more than one cell are different ?
        # total = 0
        # cell = ChessboardCell()
        # my_cell_color = target_cell_color = self.__BLANK
        # for row in range(0, 19):
        #     for col in range(0, 19):
        #         target_cell_color = target_layout.get_cell_color_col_row(col_id=col,row_id=row)
        #         if (self._layout_array[col][row] != target_cell_color):
        #             cell.from_col_row_id(col_id=col, row_id=row)
        #             my_cell_color = self._layout_array[col][row]

        #             # print('[INFO]: ChessboardMap.compare_cell_map() diff at: %s' %cell.name)
        # return total,cell.name ,my_cell_color, target_cell_color
    
    def print_out(self):
        #print('print_out(),   on debugging....')
        #return 
        int_to_char = {self.__BLANK:'. ',self._BLACK:'X ', self._WHITE:'O ',}
        print(self._FC_YELLOW + self.name)
        cell = ChessboardCell()
        # print column name on table head
        header = '    '
        for col_id in range (self._COLS, -1, -1):
            cell.from_col_row_id(col_id=col_id, row_id=1)
            header += cell.col_letter + ' '
        print(self._FC_YELLOW + header)
        # print layout row by row
        #print (self._layout_array)
        for row_id in range(0, self._ROWS, 1):
            rowNum = self._ROWS - row_id
            col_string = ''
            for col_id in range(0,self._COLS, 1):
                #print('------------- col_id, row_id', col_id, row_id)
                col_string += int_to_char[self._layout_array[18 - col_id][18 - row_id]]
            row_string = "%02d" % rowNum + '  '
            print(self._FC_YELLOW + row_string + self._FC_RESET + col_string + self._FC_YELLOW + row_string)
        print(self._FC_YELLOW + header)



    
if __name__ == "__main__":
    test1 = ChessboardLayout('Test1')
    test1.play('T19',Stone.BLACK)
    test1.print_out()
    cell = test1.get_first_cell(Stone.BLACK) 
    cell = ChessboardCell()
    x= 18
    y= 18
    cell.from_col_row_id(x,y)
    print('x=%d, y=%d, name=%s' %(cell.col_id, cell.row_id, cell.name))


    test2 = ChessboardLayout('Test2')
    test2.play('T19',Stone.BLACK)
    test2.play('K10',Stone.WHITE)
    x = test2.compare_with(test1,do_print_out=True)
    print (x)  
    
