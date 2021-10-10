from math import sqrt
from commuDevice.ble_single_client import BleSingleClient
from room_bot.room_bot_solution import RoomBotSolution
from room_bot.corner_bot_factory import RoomBotFactory
# from mpu6050 import mpu6050   # https://pypi.org/project/mpu6050-raspberrypi/
# sudo apt install python3-smbus    # need to reboot?
# sudo pip3 install mpu6050-raspberrypi
# Wiring:   https://medium.com/@kalpeshnpatil/raspberry-pi-interfacing-with-mpu6050-motion-sensor-c9608cd5f59c




class CableBotCenter:
    def __init__(self) -> None:
        # init ble
        self.__bleXPYP = BleSingleClient(RoomBotFactory.CreateCorner_XPYP())
        self.__bleXNYP = BleSingleClient(RoomBotFactory.CreateCorner_XNYP())
        self.__bleXNYN = BleSingleClient(RoomBotFactory.CreateCorner_XNYN())
        self.__bleXPYN = BleSingleClient(RoomBotFactory.CreateCorner_XPYN())
        self.__bot_solution = RoomBotSolution()
        # self.sensor = mpu6050(0x68)
        
        print("Hello world, I am CableBotCenter")


    def SpinOnce(self):
        self.__bleXPYP.SpinOnce()
        self.__bleXPYN.SpinOnce()
        self.__bleXNYN.SpinOnce()
        self.__bleXNYP.SpinOnce()

        # (a, b, g) = self.sensor.get_accel_data()


    def SendGcode(self, corner:BleSingleClient, \
            blocked_movement:bool =False,  pos:float =0.0, speed:float = 1000) \
            -> None:

        gcode = 'G1'
        if blocked_movement:
            gcode = 'G6'
        gcode += corner.server_head.AxisName + str(pos)
        gcode += 'F' + str(speed)
        corner.write_characteristic(gcode)

    def MoveTo(self, x:float, y:float, z:float):
        #  get distance
        distance = sqrt(x*x + y*y)
        for i in range(0,distance,10):
            xi = x * i / distance
            yi = y * i / distance
            zi = z * i / distance
            a,b,c,h = self.__bot_solution.IK_center_as_zero(xi,yi,zi)
            self.SendGcode(self.__bleXPYP, False, a)
            self.SendGcode(self.__bleXPYP, False, b)
            self.SendGcode(self.__bleXPYP, False, c)
            self.SendGcode(self.__bleXPYP, False, h)
            # wait all corners finished movement
            self.__bleXNYN.read_characteristic_commu(True)
            self.__bleXNYP.read_characteristic_commu(True)
            self.__bleXPYN.read_characteristic_commu(True)
            self.__bleXPYP.read_characteristic_commu(True)
            

    def HomeSingleCorner_inching(self, corner:BleSingleClient) -> bool:
        # xAngle, yAngle =self.ReadGravitySensor()
        xAngle = 0.1 
        yAngle = 0.2 
        setting = [("XPYP", 1,1),("XNYP", 1,-1),("XNYN",-1,-1),("XPYN",1,-1)]
        # Find and set the target angle pair.
        for (name, txAngle, tyAngle) in setting:
            relative_pos = -10.0
            if name == corner.server_head.BleDeviceName:
                #  Set distance to be shorter.
                pass
            else:
                # Keep the center plate being horizontal.
                self.__target_angle_x = txAngle + xAngle
                self.__target_angle_y = tyAngle + yAngle
                relative_pos = 2.0
            self.SendGcode(corner, blocked_movement=True, pos=relative_pos)
        
    def HomeSingleCorner(self, corner:BleSingleClient) -> None:
        # enter relative gcode
        corner.write_characteristic('G91')
        # read the home triger of the target corner
        corner.write_characteristic('M119')
        corner.read_characteristic_commu(True)   # Risk to stuck here?

        trigered = False
        while not trigered:  # Risk to stuck here?
            response = self.HomeSingleCorner_inching(corner)
            print('HomeSingleCorner()', response)
            if response != None:
                if response(-3,3) == 'Yes':
                    trigered = True

        # enter absolute gcode
        corner.write_characteristic('G90')
        # set current position as 0
        corner.write_characteristic('G92 ' + corner.server_head.AxisName + '0')


    def HomeAllCorners(self) -> None:
        self.HomeSingleCorner(self.__bleXPYP)
        self.HomeSingleCorner(self.__bleXPYN)
        self.HomeSingleCorner(self.__bleXNYP)
        self.HomeSingleCorner(self.__bleXNYN)

if __name__ == "__main__":
    bot = CableBotCenter()
    bot.HomeAllCorners()
    while True:
        bot.SpinOnce()
        x,y,z = int(input("input x,y,z"))
        bot.MoveTo(x,y,z)

    # while True:
    #     bot.SpinOnce()

