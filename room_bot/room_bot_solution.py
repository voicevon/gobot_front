from math import sqrt
from commuDevice. ble_single_client import BleSingleClient


class RoomBotSolution():
    def __init__(self) -> None:
        '''
        '    Positive Z if face down to ground.
        '               room_x          
        <----------------------------------------->
        XNYP                                    XPYP
        '                  
        '            <-- base_x -->      
        '            +------------+   ^
        '            |            |   |
        room_y       |   (0,0)    |   base_y 
        '            |            |   |
        '            +------------+   -
        '
        XNYN                                       XPYN
        ''' 
        self.room_x = 3600
        self.room_y = 3600
        self.base_x = 320
        self.base_y = 220

    def __get_distance(self, dx, dy, dz):
        r = sqrt(dx * dx + dy * dy)
        return sqrt(r * r + dz * dz)

    def IK_center_as_zero(self, x, y, z):
        dx = (x - self.room_x / 2 - self.base_x / 2 )
        dy = (y - self.room_y / 2 - self.base_y / 2)
        a =  self.__get_distance(dx,dy,z)

        dx = (x - self.room_x / 2 + self.base_x / 2 )
        dy = (y - self.room_y / 2 - self.base_y / 2)
        b =  self.__get_distance(dx,dy,z)

        dx = (x - self.room_x / 2 + self.base_x / 2 )
        dy = (y - self.room_y / 2 + self.base_y / 2)
        c =  self.__get_distance(dx,dy,z)

        dx = (x - self.room_x / 2 - self.base_x / 2 )
        dy = (y - self.room_y / 2 + self.base_y / 2)
        f =  self.__get_distance(dx,dy,z)

        return a,b,c,f

    def IK_left_bottom_as_zero(self, x,y,z):
        dx = (x - self.room_x - self.base_x)
        dy = (y - self.room_y - self.base_y)
        a =  self.__get_distance(dx,dy,z)

        dx = (x - self.room_x - self.base_x)
        dy = (y - self.room_y - self.base_y)
        b =  self.__get_distance(dx,dy,z)

        dx = (x - self.room_x - self.base_x)
        dy = (y - self.room_y - self.base_y)
        c =  self.__get_distance(dx,dy,z)

        dx = (x - self.room_x - self.base_x)
        dy = (y - self.room_y - self.base_y)
        f =  self.__get_distance(dx,dy,z)
   
        return a,b,c,f

