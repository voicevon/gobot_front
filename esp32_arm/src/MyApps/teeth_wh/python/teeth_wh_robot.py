

class WarehouseLocation:
    row =1
    col =2

class Message_TwhRequest():
    is_withdraw = True
    locations = [WarehouseLocation()]

    def test(self):
        self.locations[1].row

class Message_TwhGcode():
    pass

class AmqAgent():
    def __init__(self) -> None:
        pass

    def FetchMessage(self) -> Message_TwhRequest:
        request = Message_TwhRequest()
        return request

    def SendMessage(self, gcode_message):
        pass

class GcodeSender():

    def __init__(self) -> None:
        self.target_device_id = 221109

    def EefMove_Z_toTop(self):
        '''
        For Ir checking product
        '''
        pass

    def EefMove_Z_toMiddle(self, depth:int):
        '''
        For 
        '''
        pass

    def Eef_TurnOn_VacuumFan(self, is_turn_on:bool):
        pass

    def Eef_EnableSuck(self, is_enable:bool):
        pass

    def PauseForWaiting(self, second: int):
        pass


    def MoveArmToCenter(self):
        g1 = "G1A0"


    def MoveTo(self, x, y):
        g1 = "G!X" + str(x) + "Y" + str(y)


class RobotCerebellum():
    def __init__(self) -> None:
        # self.map = RobotMap()
        self.gcode_sender = GcodeSender()
        pass

    def GetXY_FromRowCol(self, location: WarehouseLocation):
        x = location.row * 40
        y = location.col * 40
        return x,y

    def PickupAndCheck(self, location: WarehouseLocation):
        self.gcode_sender.EefMove_Z_toTop()
        x,y = self.GetXY_FromRowCol(location)
        self.gcode_sender.MoveTo(x,y)
        self.gcode_sender.EefMove_Z_toMiddle(12)
        self.gcode_sender.Eef_EnableSuck(True)
        self.gcode_sender.EefMove_Z_toTop()

    def Dropto_CenterBox(self):
        self.gcode_sender.MoveArmToCenter()
        self.gcode_sender.Eef_EnableSuck(False)  # drop the tooth to out-box
        self.gcode_sender.PauseForWaiting(3)
        self.gcode_sender.Eef_EnableSuck(False)

class TeethWarehouseRobot():
    def __init__(self) -> None:
        self.cerebellum = RobotCerebellum()

    def ExecCommand(self, request: Message_TwhRequest) -> bool:
        '''
        This should start a new thread.
        Reason: Dynamic gcode follow a  branched processing flow. Synced with hardware robot. 
                And take a long time up to couple minutes.
        '''
        if request.is_withdraw:
            index = 0
            while index <request.locations.count():
                # clear message on broker.
                pickup_state = 'todo'
                while pickup_state != 'done':
                    if pickup_state == 'todo':
                        self.cerebellum.PickupAndCheck(request.locations[index].row, request.locations[index].col)
                        pickup_state = 'doing'
                    if pickup_state == 'doing':
                        #check message, and update suck_state
                        if (1==1):
                            # got message:  ir is blocked
                            pickup_state = 'done'
                        if (2==3):
                            # got message:  ir is passing through.
                            pickup_state = 'todo'

                self.cerebellum.Dropto_CenterBox()
                # checking weight before-dropping
            
                # # checking weight after-dropping
                # if(False):
                #     #weight is not increased.  
                #     pass
                # if (False):
                #     # Weight is increased too much.
                #     pass
                index += 1

            # All the location in request is done. 
            # Now, send a message to inform user:  "Your request is dealed. Please pick up the bag."

if __name__ == '__main__':
    robot = TeethWarehouseRobot()
    while True:
        request = AmqAgent.FetchMessage()
        if request is not None:
            robot.ExecCommand(request)
