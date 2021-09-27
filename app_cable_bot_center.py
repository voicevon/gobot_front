from commuDevice. ble_single_client import BleSingleClient
from config.config_cable_bot_center import CornerBotFactory



class CableBotCenter:
    def __init__(self) -> None:
        # init ble
        self.__bleXPYP = BleSingleClient(CornerBotFactory.CreateCorner_XPYP())
        self.__bleXNYP = BleSingleClient(CornerBotFactory.CreateCorner_XNYP())
        self.__bleXNYN = BleSingleClient(CornerBotFactory.CreateCorner_XNYN())
        self.__bleXPYN = BleSingleClient(CornerBotFactory.CreateCorner_XPYN())
        print("Hello world, I am CableBotCenter")


    def SpinOnce(self):
        self.__bleXPYP.SpinOnce()
        self.__bleXPYN.SpinOnce()
        self.__bleXNYN.SpinOnce()
        self.__bleXNYP.SpinOnce()

        x, y = self.ReadGravitySensor()

    def ReadGravitySensor(self):
        x = 0
        y = 1
        return x,y

    def SendGcode(self, corner: BleSingleClient, pos: float) -> None:
        gcode = 'G1 ' + corner.__server.AxisName + str(pos)
        BleSingleClient.write_characteristic(gcode)

    def IK(self, x, y, z):
        a = 100 + x
        b = 200 + y
        c = 300 + z
        f = 400
        return a,b,c,f

    def MoveTo(self, x, y, z):
        a,b,c,f = self.IK(x,y,z)
        self.SendGcode(self.__bleXPYP,a)
        self.SendGcode(self.__bleXPYP,b)
        self.SendGcode(self.__bleXPYP,c)
        self.SendGcode(self.__bleXPYP,f)

    def HomeSingleCorner(self, corner: BleSingleClient) -> None:
        xAngle,yAngle =self.ReadGravitySensor()
        setting = {("XPYP", 1,1),("XNYP", 1,-1),("XNYN",-1,-1),("XPYN",1,-1)}
        # Find and set the target angle pair.
        for (name,txAngle,tyAngle) in setting:
            if name == corner.__server.BleDeviceName:
                self.__target_angle_x = txAngle + xAngle
                self.__target_angle_y = tyAngle + yAngle
            else:
                self.__target_angle_x = txAngle + xAngle
                self.__target_angle_y = tyAngle + yAngle

        self.SendGcode()

    def HomeAllCorners(self):
        self.HomeSingleCorner(self.__bleXPYP)
        self.HomeSingleCorner(self.__bleXPYN)
        self.HomeSingleCorner(self.__bleXNYP)
        self.HomeSingleCorner(self.__bleXNYN)

if __name__ == "__main__":
    bot = CableBotCenter()
    bot.HomeAllCorners()

