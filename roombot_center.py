import time
from math import sqrt
# from commuDevice.ble_single_client import BleServerHead
from room_bot.corner_agent import CornerAgent
from room_bot.room_bot_solution import RoomBotSolution
from room_bot.corner_factory import RoomBot_CornerFactory
from commuDevice.gcode_sender import GcodeFactory
# from mpu6050 import mpu6050   # https://pypi.org/project/mpu6050-raspberrypi/
# sudo apt install python3-smbus    # need to reboot?
# sudo pip3 install mpu6050-raspberrypi
# Wiring:   https://medium.com/@kalpeshnpatil/raspberry-pi-interfacing-with-mpu6050-motion-sensor-c9608cd5f59c
import logging



class CableBotCenter:
    def __init__(self) -> None:
        # init ble
        self.__XPYP = CornerAgent(RoomBot_CornerFactory.CreateServerHead_XPYP())
        self.__XNYP = CornerAgent(RoomBot_CornerFactory.CreateServerHead_XNYP())
        self.__XNYN = CornerAgent(RoomBot_CornerFactory.CreateServerHead_XNYN())
        self.__XPYN = CornerAgent(RoomBot_CornerFactory.CreateServerHead_XPYN())
        self.__bot_solution = RoomBotSolution()
        # self.sensor = mpu6050(0x68)
        
        print("Hello world, I am CableBotCenter")


    def SpinOnce(self):
        self.__XPYP.SpinOnce()
        self.__XPYN.SpinOnce()
        self.__XNYN.SpinOnce()
        self.__XNYP.SpinOnce()

        # (a, b, g) = self.sensor.get_accel_data()



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
            

    def HomeSingleCorner_inching(self, corner:CornerAgent) -> bool:
        # xAngle, yAngle =self.ReadGravitySensor()
        xAngle = 0.1 
        yAngle = 0.2 
        setting = [("XPYP", 1,1),("XNYP", 1,-1),("XNYN",-1,-1),("XPYN",1,-1)]
        # Find and set the target angle pair.
        for (name, txAngle, tyAngle) in setting:
            relative_pos = -10.0
            if name == corner.commu_device.server_head.AxisName:
                #  Set distance to be shorter.
                pass
            else:
                # Keep the center plate being horizontal.
                self.__target_angle_x = txAngle + xAngle
                self.__target_angle_y = tyAngle + yAngle

            gcode = GcodeFactory()
            gcode.AxisName = corner.commu_device.server_head.AxisName
            gcode.IsAbsolutePosition = False
            gcode.BlockMovement = True
            gcode.Speed = 1000.0
            gcode.TargetPosision_or_distance = 10.0
            corner.append_gcode(gcode)
        
    def HomeSingleCorner(self, corner:CornerAgent) -> None:
        # enter relative gcode
        corner.append_gcode('G91')
        # read the home triger of the target corner
        corner.append_gcode('M119')
        # corner.read_characteristic_commu(True)   # Risk to stuck here?

        trigered = False
        while not trigered:  # Risk to stuck here?
            response = self.HomeSingleCorner_inching(corner)
            print('HomeSingleCorner()', response)
            if response != None:
                if response(-3,3) == 'Yes':
                    trigered = True
            else:
                time.sleep(1)


    def HomeAllCorners(self) -> None:
        self.HomeSingleCorner(self.__XPYP)
        self.HomeSingleCorner(self.__XPYN)
        self.HomeSingleCorner(self.__XNYP)
        self.HomeSingleCorner(self.__XNYN)

if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    bot = CableBotCenter()
    bot.HomeAllCorners()
    while True:
        bot.SpinOnce()
        x,y,z = int(input("input x,y,z"))
        bot.MoveTo(x,y,z)

    # while True:
    #     bot.SpinOnce()

