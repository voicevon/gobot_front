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

    def MoveTo(self, x, y, z):
        a,b,c,f = IK(x,y,z)
        self.send_gcode(self.__bleXPYP,a)
        self.send_gcode(self.__bleXPYP,b)
        self.send_gcode(self.__bleXPYP,c)
        self.send_gcode(self.__bleXPYP,f)

    def ReadGravitySensor(self):
        x = 0
        y = 1
        return x,y

if __name__ == "__main__":
    bot = CableBotCenter()
    bot.HomeAllCorners()

