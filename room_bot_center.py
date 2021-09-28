from commuDevice. ble_single_client import BleSingleClient
from room_bot.room_bot_solution import RoomBotSolution
from room_bot.corner_bot_factory import RoomBotFactory
from mpu6050 import mpu6050   # https://pypi.org/project/mpu6050-raspberrypi/
# sudo apt install python3-smbus
# pip3 install mpu6050-raspberrypi


class CableBotCenter:
    def __init__(self) -> None:
        # init ble
        self.__bleXPYP = BleSingleClient(RoomBotFactory.CreateCorner_XPYP())
        self.__bleXNYP = BleSingleClient(RoomBotFactory.CreateCorner_XNYP())
        self.__bleXNYN = BleSingleClient(RoomBotFactory.CreateCorner_XNYN())
        self.__bleXPYN = BleSingleClient(RoomBotFactory.CreateCorner_XPYN())
        self.__bot_solution = RoomBotSolution()
        self.sensor = mpu6050(0x68)
        
        print("Hello world, I am CableBotCenter")


    def SpinOnce(self):
        self.__bleXPYP.SpinOnce()
        self.__bleXPYN.SpinOnce()
        self.__bleXNYN.SpinOnce()
        self.__bleXNYP.SpinOnce()

        (a, b, g) = self.sensor.get_accel_data()


    def SendGcode(self, corner: BleSingleClient, pos: float) -> None:
        gcode = 'G1 ' + corner.server.AxisName + str(pos)
        corner.write_characteristic(gcode)

    def MoveTo(self, x, y, z):
        a,b,c,f = self.__bot_solution.IK(x,y,z)
        self.SendGcode(self.__bleXPYP,a)
        self.SendGcode(self.__bleXPYP,b)
        self.SendGcode(self.__bleXPYP,c)
        self.SendGcode(self.__bleXPYP,f)

    def HomeSingleCorner(self, corner: BleSingleClient) -> None:
        xAngle, yAngle =self.ReadGravitySensor()
        setting = {("XPYP", 1,1),("XNYP", 1,-1),("XNYN",-1,-1),("XPYN",1,-1)}
        # Find and set the target angle pair.
        for (name, txAngle, tyAngle) in setting:
            if name == corner.server.BleDeviceName:
                self.__target_angle_x = txAngle + xAngle
                self.__target_angle_y = tyAngle + yAngle
                pos = 1
            else:
                self.__target_angle_x = txAngle + xAngle
                self.__target_angle_y = tyAngle + yAngle
                pos = 2
            self.SendGcode(corner,pos)

    def HomeAllCorners(self):
        self.HomeSingleCorner(self.__bleXPYP)
        self.HomeSingleCorner(self.__bleXPYN)
        self.HomeSingleCorner(self.__bleXNYP)
        self.HomeSingleCorner(self.__bleXNYN)

if __name__ == "__main__":
    bot = CableBotCenter()
    bot.HomeAllCorners()
    while True:
        bot.SpinOnce()

