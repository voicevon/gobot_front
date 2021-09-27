from math import sqrt
from commuDevice. ble_single_client import BleSingleClient
from room_bot.gravity_sensor import MMU6050


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
        self.__gravity_sensor = MMU6050()

    def IK(self, x, y, z):
        a1 = (x + self.room_x / 2 + self.base_x / 2 )
        a =  sqrt(a1 * a1 + z * z)
        
        b1 = (x + self.room_x / 2 + self.base_x / 2 )
        b =  sqrt(b1 * b1 + z * z)
        
        c1 = (x + self.room_x / 2 + self.base_x / 2 )
        c =  sqrt(c1 * c1 + z * z)
        
        f1 = (x + self.room_x / 2 + self.base_x / 2 )
        f =  sqrt(f1 * f1 + z * z)

        return a,b,c,f



