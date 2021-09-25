


class CableBotCenter:
    def __init__(self) -> None:
        # init ble
        pass

    def SpinOnce(self):
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

