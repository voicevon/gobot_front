import enum

from Pylib.robot_map import MapSite
from gogame.chessboard_cell import ChessboardCell


class ArmMapSite_Catalog(enum.Enum):
    TRASH_BIN_BLACK = 1,
    TRASH_BIN_WHITE = 2,
    HOUSE_VENDOR = 3,
    CHESSBOARD_CELL = 4,
    PARKING = 5

class ArmMapSiteFactory():
    def __init__(self) -> None:
        self.house_vendor = MapSite('house_vendor', 0, 80)
        self.trash_bin_white = MapSite('trash_bin_white', -230, 0)
        self.trash_bin_black = MapSite('trash_bin_black', 230, 0)
        self.parking = MapSite('Parking', 0, 80)
        # self.CELL_A1 = MapSite("CELL_A1", -28 * 9, 155 + 23.8 * 18)
        # self.CELL_T19 = MapSite("CELL_T19", 28 * 9 , 155)

    def MakeSingleSite(self, cat:ArmMapSite_Catalog, cell:ChessboardCell=None) -> MapSite:
        '''
        TODO:  static or class method.  What different?
        '''
        if cat == ArmMapSite_Catalog.HOUSE_VENDOR:
            return self.house_vendor
        elif cat == ArmMapSite_Catalog.TRASH_BIN_WHITE:
            return self.trash_bin_white
        elif cat == ArmMapSite_Catalog.TRASH_BIN_BLACK:
            return self.trash_bin_black
        elif cat == ArmMapSite_Catalog.CHESSBOARD_CELL:
            x, y = self.__Get_XY_FromCell(cell)
            cell_site = MapSite("Q1", x, y)
            return cell_site
        else:
            print("[Error] ArmMapSites.GetSingleSite()  cat=, index= ", cat, cell)


    def __Get_XY_FromCell(self, cell:ChessboardCell):
        '''
        Go_position instance: "Q4" as position on 2D Chessboard.
        XY_position instance: "[0.015,0.02,-0.01]" as [x,y,z] in 3D world coordinator.
        '''
        # ss = "ABCDEFGHIJKLMNOPQRST"
        # print(cell.name)
        # iCol = ss.find(cell_name[:1])
        # iRow = 19 - int(cell_name[1:])
        zero_in_world_x = - 404.0 / 18 * 9
        zero_in_world_y = 155
        cell_width_x = 404.0 / 18
        cell_width_y = 430.0 / 18
        x = zero_in_world_x +  cell_width_x * cell.col_id
        y = zero_in_world_y +  cell_width_y * cell.row_id
        return x, y


