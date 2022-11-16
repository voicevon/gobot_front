

class WarehouseLocation:
    row =1
    col =2

class TwhRequest():
    is_withdraw = True
    locations = [WarehouseLocation()]

    def test(self):
        self.locations[1].row


class AmqAgent():
    def __init__(self) -> None:
        pass

    def FetchMessage() -> TwhRequest:
        request = TwhRequest()
        return request


class GcodeSender():

    def __init__(self) -> None:
        pass

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



# class RobotMap():
#     def __init__(self) -> None:
#         pass



class RobotCerebellum():
    def __init__(self) -> None:
        # self.map = RobotMap()
        pass

    def GetXY_FromRowCol(self, row:int, col:int):
        x = 123
        y = 234
        return x,y

    def MoveTo(self, x, y):
        pass

    def MoveArmToCenter(self):
        pass

class TeethWarehouseRobot():
    def __init__(self) -> None:
        self.gcode_sender = GcodeSender()
        self.cerebellum = RobotCerebellum()

    def ExecCommand_PickupAndCheck(self, row, col):
        self.gcode_sender.EefMove_Z_toTop()
        x,y = self.cerebellum.GetXY_FromRowCol(row, col)
        self.cerebellum.MoveTo(x,y)
        self.gcode_sender.EefMove_Z_toMiddle(12)
        self.gcode_sender.Eef_EnableSuck(True)
        self.gcode_sender.EefMove_Z_toTop()

    def ExecCommand(self, request: TwhRequest) -> bool:
        '''
        This should start a new thread.
        '''
        if request.is_withdraw:
            index = 0
            # clear message on broker.
            pickup_state = 'todo'
            while pickup_state != 'done':
                if pickup_state == 'todo':
                    self.ExecCommand_PickupAndCheck(request.locations[index].row, request.locations[index].col)
                    pickup_state = 'doing'
                if pickup_state == 'doing':
                    #check message, and update suck_state
                    if (1==1):
                        # got message:  ir is blocked
                        pickup_state = 'done'
                    if (2==3):
                        # got message:  ir is passing through.
                        pickup_state = 'todo'

            self.cerebellum.MoveArmToCenter()
            self.gcode_sender.Eef_EnableSuck(False)  # drop one tooth to out-box
            self.gcode_sender.PauseForWaiting(3)
            self.gcode_sender.Eef_EnableSuck(False)
            # teeth droped from air pen to out-box

            # checking weight before-dropping
            
            # checking weight after-dropping
            if(False):
                #weight is not increased.  
                pass
            if (False):
                # Weight is increased too much.
                pass


if __name__ == '__main__':
    robot = TeethWarehouseRobot()
    while True:
        request = AmqAgent.FetchMessage()
        if request is not None:
            robot.ExecCommand(request)
