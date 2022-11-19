from von.rabbitmq_agent import RabbitMqAgent, AMQ_BrokerConfig
import json






class GcodeSender():

    def __init__(self) -> None:
        self.target_device_id = 221109
        self.exchange_name = 'twh'
        self.queue_name = self.exchange_name + '_' + str(self.target_device_id) + '_gcode'

    def EefMove_Z_toTop(self):
        '''
        For Ir checking product
        '''
        print('gcode ','EefMove_Z_toTop')
        pass

    def EefMove_Z_toMiddle(self, depth:int):
        '''
        For 
        '''
        print('gcode ','EefMove_Z_toMiddle')
        pass

    def Eef_TurnOn_VacuumFan(self, is_turn_on: bool):
        print('gcode ','Eef_TurnOn_VacuumFan')
        pass

    def Eef_EnableSuck(self, is_enable:bool):
        print('gcode ','Eef_EnableSuck')
        pass

    def PauseForWaiting(self, second: int):
        print('gcode ','PauseForWaiting')
        pass


    def MoveArmToCenter(self):
        print('gcode ','MoveArmToCenter')
        g1 = "G1A0"


    def MoveTo(self, x, y):
        print('gcode ','MoveTo')
        g1 = "G!X" + str(x) + "Y" + str(y)
        g_amq.Publish(self.exchange_name, self.queue_name, g1)


class RobotCerebellum():
    def __init__(self) -> None:
        # self.map = RobotMap()
        self.gcode_sender = GcodeSender()
        pass

    def GetXY_FromRowCol(self, row, col):
        x = row * 40
        y = col * 40
        return x,y

    def PickupAndCheck(self, row: int, col: int):
        self.gcode_sender.EefMove_Z_toTop()
        x,y = self.GetXY_FromRowCol(row=row, col=col)
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
        self.state = 'idle'


    def SpinOnce(self):
        if self.state == 'idle':
            return
        if self.state == 'picking_cell':
            if 1==1:
                #IR checking is ready
                self.state =='to_be_checked'
        if self.state == 'to_be_checked':
            if 2==2:
                # passed checking
                self.state = 'droping_box'
            else:
                #failed checking
                self.state = 'picking_cell'
        if self.state == 'droping_box':
            self.state = 'idle'

    def ExecCommand(self, command: str) -> bool:
        '''
        This should start a new thread.
        Reason: Dynamic gcode follow a  branched processing flow. Synced with hardware robot. 
                And take a long time up to couple minutes.
        '''
        print(command)
        if command == '"withdraw_end"':
            print('withdraw_end')
        else:
            request = json.loads(command)
            # while index <request.locations.count():
                # clear message on broker.
            if request['is_withdraw']:
                self.pickup_state = 'todo'
                while self.pickup_state != 'done':
                    if self.pickup_state == 'todo':
                        self.cerebellum.PickupAndCheck(request['row'], request['col'])
                        self.pickup_state = 'doing'
                    if self.pickup_state == 'doing':
                        #check message, and update suck_state
                        if (1==1):
                            # got message:  ir is blocked
                            self.pickup_state = 'done'
                        if (2==3):
                            # got message:  ir is passing through.
                            self.pickup_state = 'todo'
                else:
                    #do deposite
                    pass

                self.cerebellum.Dropto_CenterBox()
                # checking weight before-dropping
            
                # # checking weight after-dropping
                # if(False):
                #     #weight is not increased.  
                #     pass
                # if (False):
                #     # Weight is increased too much.
                #     pass
                # index += 1

            # All the location in request is done. 
            # Now, send a message to inform user:  "Your request is dealed. Please pick up the bag."

robot = TeethWarehouseRobot()


if __name__ == '__main__':
    g_amq = RabbitMqAgent()
    amq_broke_config = AMQ_BrokerConfig()
    g_amq.connect_to_broker(amq_broke_config)
    g_amq.Subscribe("twh_221109_request")
    
    while True:
        g_amq.SpinOnce()
        user_request = g_amq.FetchMessage()
        if user_request is not None:
            robot.ExecCommand(user_request.decode('utf-8'))

