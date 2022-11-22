from von.rabbitmq_agent import g_amq


class GcodeSender():

    def __init__(self) -> None:
        self.target_device_id = 221109
        self.exchange_name = 'twh'
        self.queue_name = self.exchange_name + '_' + str(self.target_device_id) + '_gcode'

    def home_alpha(self):
        g28 = 'G28A'
        g_amq.Publish(self.exchange_name, self.queue_name, g28)

    def home_x(self):
        g28 = 'G28X'
        g_amq.Publish(self.exchange_name, self.queue_name, g28)

    def move_centerbox_to_outside(self):
        g1 = 'G1X1200'
        g_amq.Publish(self.exchange_name, self.queue_name, g1)

    def move_centerbox_to_inside(self):
        g1 = 'G1X400'
        g_amq.Publish(self.exchange_name, self.queue_name, g1)

    def pickup_from_centerbox(self):
        # move to top
        g1 = 'G1Z0'
        g_amq.Publish(self.exchange_name, self.queue_name, g1)
        self.Eef_EnableSuck(True)
        # arm to center position
        g1 = 'G1A0'
        g_amq.Publish(self.exchange_name, self.queue_name, g1)
        # move down
        g1 = 'G1Z100'
        g_amq.Publish(self.exchange_name, self.queue_name, g1)
        g4 = 'G4S2'
        g_amq.Publish(self.exchange_name, self.queue_name, g4)
        # move up
        g1 = 'G1Z0'
        g_amq.Publish(self.exchange_name, self.queue_name, g1)
        g4 = 'G4S2'
        g_amq.Publish(self.exchange_name, self.queue_name, g4)
        
    def drop_to_cellbox(self, row, col):
        self.MoveTo(row * 40, col * 40)
        self.Eef_EnableSuck(False)



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
        g1 = "G1X" + str(x) + "Y" + str(y)
        g_amq.Publish(self.exchange_name, self.queue_name, g1)

    def move_x_to(self, x):
        print('gcode ','MoveTo')
        g1 = "G1X" + str(x) 
        g_amq.Publish(self.exchange_name, self.queue_name, g1)

    def move_a_to(self, a):
        print('gcode ','MoveTo')
        g1 = "G1A" + str(a) 
        g_amq.Publish(self.exchange_name, self.queue_name, g1)

    def move_xy_to(self, x, y):
        print('gcode ','MoveTo')
        g1 = "G1X" + str(x) + "Y" + str(y)
        g_amq.Publish(self.exchange_name, self.queue_name, g1)