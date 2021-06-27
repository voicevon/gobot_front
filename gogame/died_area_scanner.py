from gogame.chessboard import ChessboardLayout
from gogame.chessboard_cell import ChessboardCell
class DiedAreaScanner(ChessboardLayout):
    def __init__(self):
        ChessboardLayout.__init__(self,'Died area scanner')
        self.__died_area_array = [([0] * 19) for i in range(19)]

        # self.__DIED_BLACK = self._BLACK + 100
        # self.__DIED_WHITE = self._WHITE + 100

    def __is_around_alived(self, col, row):
        '''
        # look neibours is alive or not.
        #       A cell might have 4 neibours,  TODO: 3 neibours, or 2 neibours
        # return:
        #       15 = has been waken up, because this cell is blank 
        #       16 = has been waken up, because this cell is connected to #15
        #       17 = has benn waken up, because this cell is connected to #16
        '''

        # look up,down,left,right. 
        if row > 0:
            s = self.__died_area_array[col][row - 1]
            if s > 11:
                # upper cell is blank
                return s + 1
        if col > 0:
            s = self.__died_area_array[col - 1][row]
            if s > 11:
                # left cell is blank
                return s + 1
        if row < 18:
            s = self.__died_area_array[col][row + 1]
            if s > 11:
                # lower cell is blank
                return s + 1
        if col < 18:
            s = self.__died_area_array[col + 1][row]
            if s > 11:
                # righ cell is blank
                return s + 1
        return 0

    def __try_to_make_alive(self, col, row):
        '''
        # based on neighbor's value:
        #       15 = has been waken up, because this cell is blank 
        #       16 = has been waken up, because this cell is connected to #15
        # my value will be:
        #       16 = has been waken up, because this cell is connected to #15
        #       17 = has benn waken up, because this cell is connected to #16
        '''
        connecting_type = self.__is_around_alived(col,row)
        if connecting_type > 11:
            self.__died_area_array[col][row] = connecting_type
            return connecting_type

        return 0

    def __backword_loop(self):
        c = 0
        for col in range(18,-1,-1):
            for row in range(18,-1,-1):
                if self.__died_area_array[col][row] == 0:
                    # Yes, this cell wants to be waken up
                    wake_up_type = self.__is_around_alived(col,row)
                    if wake_up_type > 11:
                        c += 1
                        if wake_up_type > 17:
                            wake_up_type = 17
                        self.__died_area_array[col][row] = wake_up_type 
        return c

    def __forward_loop(self):
        count = 0
        for col in range(0,19):
            for row in range(0,19):
                if self.__died_area_array[col][row] == 0:
                    # Yes, this cell wants to be waken up
                    wake_up_type = self.__try_to_make_alive(col=col, row=row)
                    if wake_up_type > 11:
                        count += 1
                        if wake_up_type > 17:
                            wake_up_type = 17
                        self.__died_area_array[col][row] = wake_up_type
        return count

    def start_scan(self, target_color):
        '''
        return:
            count: How many cells are died

        # 0 = assume cell is died
        # 1 = No need to be waken up, because this cell is opposit color
        # 15 = has been wakup up, because this cell is blank
        '''
        self.__init_died_area_array(self._layout_array, target_color)
        # print('inited')
        # self.print_out()

        count = 9999
        while count > 0:
            count = 0 
            count += self.__forward_loop()
            # self.print_out()
            count += self.__backword_loop()
            # self.print_out()

        count = 0
        for col in range(0, self._COLS):
            for row in range(0, self._ROWS):
                if self.__died_area_array[col][row] == 0:
                    count += 1
        return count

    def print_out_died_area(self):
        int_to_char = {0:'x ', 1:'O ', 15:'. ', 16:'- ', 17:'* ', 8:'B ', 3:'W '}
        print(self._FC_YELLOW + 'Died area')
        cell = ChessboardCell()
        # print column name on table head
        header = '    '
        for col_id in range (19,-1,-1):
            cell.from_col_row_id(col_id=col_id, row_id=1)
            header += cell.col_letter + ' '
        print(self._FC_YELLOW + header)
        # print layout row by row
        for row_id in range(0, 19):
            rowNum = 19 - row_id
            col_string = ''
            for col_id in range(0,19):
                value = self.__died_area_array[18 - col_id][18 - row_id]
                if value == 0:
                    col_string += self._BG_RED + int_to_char[value] + self._FC_RESET
                else:
                    col_string += int_to_char[value]

            row_string = "%02d" % rowNum + '  '
            print (self._FC_YELLOW + row_string + self._FC_RESET + col_string + self._FC_YELLOW + row_string)
        print(self._FC_YELLOW + header)

    def get_first_died_cell(self):
        '''
        return:
            None means not found!
        '''
        cell = ChessboardCell()
        for row_id in range(0, self._ROWS):
            for col_id in range(0, self._COLS):
                if self.__died_area_array[col_id][row_id] == 0:
                    cell.from_col_row_id(col_id,row_id)
                    return cell
        return None

    def died_cell_removed_first_one(self):
        cell = ChessboardCell()
        cell = self.get_first_died_cell()
        self.__died_area_array[cell.col_id][cell.row_id] = 15

    def __init_died_area_array(self, origin_array,target_color_code):
        '''
        # Based on origin_array
        #       0 = Blank
        #       3 = White
        #       8 = Black
        # init self.__died_area_array
        #       0 = assume cell is died
        #       1 = No need to be waken up, because this cell is opposit color
        #       15 = has been wakup up, because this cell is blank
        '''
        # assume all target_color on the board is died.
        for col in range(0,19):
            for row in range(0,19):
                value = origin_array[col][row]
                if value == 0:
                    # blank cell
                    self.__died_area_array[col][row] = 15
                elif value == target_color_code:
                    self.__died_area_array[col][row] = 0
                else:
                    # oppsite color
                    self.__died_area_array[col][row] = 1

    def set_layout_array(self,layout_array):
        self._layout_array = layout_array
