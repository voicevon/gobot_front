from math import sqrt
from commuDevice.ble_single_client import BleConnState, BleConnection
# from commuDevice.ble_single_client import BleServerHead
from room_bot.corner_agent import CornerAgent
from room_bot.room_bot_solution import RoomBotSolution
from room_bot.corner_factory import RoomBot_CornerFactory
from commuDevice.gcode_factory import GcodeFactory
# from mpu6050 import mpu6050   # https://pypi.org/project/mpu6050-raspberrypi/
# sudo apt install python3-smbus    # need to reboot?
# sudo pip3 install mpu6050-raspberrypi
# Wiring:   https://medium.com/@kalpeshnpatil/raspberry-pi-interfacing-with-mpu6050-motion-sensor-c9608cd5f59c
import logging
import time
import board
import adafruit_mpu6050



class CableBotCenter:
    def __init__(self) -> None:
        # init ble
        self.__XPYP = CornerAgent(RoomBot_CornerFactory.CreateServerHead_XPYP())
        self.__XNYP = CornerAgent(RoomBot_CornerFactory.CreateServerHead_XNYP())
        self.__XNYN = CornerAgent(RoomBot_CornerFactory.CreateServerHead_XNYN())
        self.__XPYN = CornerAgent(RoomBot_CornerFactory.CreateServerHead_XPYN())
        self.__bot_solution = RoomBotSolution()
        # self.sensor = mpu6050(0x68)
        # i2c = board.I2C()
        # mpu = adafruit_mpu6050.MPU6050(i2c)

        i2c = board.I2C()  # uses board.SCL and board.SDA
        self.mpu6050 = adafruit_mpu6050.MPU6050(i2c)
        print("Hello world, I am CableBotCenter")


    def SpinOnce(self):
        self.__XPYP.SpinOnce()
        self.__XPYN.SpinOnce()
        self.__XNYN.SpinOnce()
        self.__XNYP.SpinOnce()

        # (a, b, g) = self.sensor.get_accel_data()
        # print("Acceleration: X:%.2f, Y: %.2f, Z: %.2f m/s^2" % (self.mpu6050.acceleration))
        # print("Gyro X:%.2f, Y: %.2f, Z: %.2f rad/s" % (self.mpu6050.gyro))
        # print("Temperature: %.2f C" % self.mpu6050.temperature)
        # print(" ")
        # a,b,g = self.mpu6050.acceleration()
        # print(a,b,g)
        # time.sleep(1)


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
            # self.__bleXNYN.read_characteristic_commu(True)
            # self.__bleXNYP.read_characteristic_commu(True)
            # self.__bleXPYN.read_characteristic_commu(True)
            # self.__bleXPYP.read_characteristic_commu(True)
            

    def HomeSingleCorner_inching(self, corner:CornerAgent) -> bool:
        # xAngle, yAngle =self.ReadGravitySensor()
        xa,ya,za = self.mpu6050.acceleration()
        print('Reading Angle', xa, ya, za)
        setting = [("XPYP", 1,1),("XNYP", 1,-1),("XNYN",-1,-1),("XPYN",1,-1)]
        # Find and set the target angle pair.
        for (name, xDir, yDir) in setting:
            gcode = GcodeFactory()
            gcode.AxisName = corner.commu_device.server_head.AxisName
            gcode.BlockMovement = True

            if name == corner.commu_device.server_head.AxisName:
                #  Set distance to be shorter.
                gcode.TargetPosision_or_distance = -10.0
            else:
                # Keep the center plate being horizontal.
                real_angle = xa*xDir + ya*yDir
                if real_angle >3:
                    # Angle is too over much, rope should be shorter
                    gcode.TargetPosision_or_distance = -10
                if real_angle < -3:
                    # Angle is too over less, rope should be longer
                    gcode.TargetPosision_or_distance = 10

            corner.append_gcode(gcode)


    def HomeSingleCorner(self, corner:CornerAgent) -> None:
        corner.wait_robot_be_idle_M280()
        
        triggered = False
        while not triggered:  # Risk to stuck here?
            self.HomeSingleCorner_inching(corner)
            # read the home triger of the target corner
            got_response = False
            while not got_response:
                corner.append_gcode_string('M119')
                response = corner.commu_device.read_characteristic_state()
                print('HomeSingleCorner()', response)
                if response == None:
                    got_response= False
                elif response[-3:] == 'Yes':
                    got_response = True
                    triggered = True
                elif response[-2:] == 'No':
                    got_response = True
                else:
                    print('Roombot_Center.HomeSingleCorner()   Unexpected response ', response)

    def HomeAllCorners(self) -> None:
        self.__XPYP.append_gcode_string('G91')
        self.__XPYN.append_gcode_string('G91')
        self.__XNYN.append_gcode_string('G91')
        self.__XNYP.append_gcode_string('G91')
        self.HomeSingleCorner(self.__XPYP)
        print("XPYP  is homed")

        self.HomeSingleCorner(self.__XPYN)
        self.HomeSingleCorner(self.__XNYP)
        self.HomeSingleCorner(self.__XNYN)

        self.__XPYP.append_gcode_string('G90')
        self.__XPYN.append_gcode_string('G90')
        self.__XNYN.append_gcode_string('G90')
        self.__XNYP.append_gcode_string('G90')

    def ConnectAllCorners(self) -> bool:
        all_corners_is_connected = False
        while not all_corners_is_connected:
            self.SpinOnce()
            all_corners_is_connected = True
            if self.__XPYP.commu_device.connection.state != BleConnState.CONNECTED:
                all_corners_is_connected = False
            if self.__XPYN.commu_device.connection.state != BleConnState.CONNECTED:
                all_corners_is_connected = False
            if self.__XNYP.commu_device.connection.state != BleConnState.CONNECTED:
                all_corners_is_connected = False
            if self.__XNYN.commu_device.connection.state != BleConnState.CONNECTED:
                all_corners_is_connected = False
        logging.info("All Corners are connected!")


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    bot = CableBotCenter()
    bot.ConnectAllCorners()

    # bot.HomeAllCorners()
    while True:
        bot.SpinOnce()
        x,y,z = int(input("input x,y,z"))
        bot.MoveTo(x,y,z)

    # while True:
    #     bot.SpinOnce()

