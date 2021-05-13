
# from typing_extensions import runtime_checkable


class Scanner():
    def __init__(self, board_type):
        '''   
        Supported board_types:
            Warehouse: Four aruco marks
            Commander: One aruco mark.
            Go_game_board_19x19: Normally two aruco marks
                                 Calibrate mode, Four marks
        '''
        self.__board_type = board_type


    def get_stone_postion(self, img, color):
        '''
                 y
                 ^
                 |
                 |
        ---------+---------> x
        '''
        color = 'BLACK'
        for y in range(50,0,-1):
            for x in range (-50,50):
                if True:
                    # Got black stone
                    return x, y
        return None,None
