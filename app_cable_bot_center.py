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
        
    def HomeSingleCorner(self, corner):
        xAngle,yAngle =self.ReadGravitySensor()
        setting = {("XPYP", 1,1),("XNYP", 1,-1),("XNYN",-1,-1),("XPYN",1,-1)}
        # Find and set the target angle pair.
        for (name,txAngle,tyAngle) in setting:
            if corner== name:
                
                self.__target_angle_x = txAngle + xAngle
                self.__target_angle_y = tyAngle + yAngle

        
        self.SendGcode()


    def ReadGravitySensor(self):
        x = 0
        y = 1
        return x,y

if __name__ == "__main__":
    bot = CableBotCenter()
