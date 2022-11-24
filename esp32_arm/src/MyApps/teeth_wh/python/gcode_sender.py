from von.rabbitmq_agent import g_amq


class GcodeSender():

    def __init__(self) -> None:
        self.target_device_id = 221109
        self.exchange_name = 'twh'
        self.queue_name = self.exchange_name + '_' + str(self.target_device_id) + '_gcode'  # twh_221109_gcode

    def home_alpha(self):
        g28 = 'G28a'
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
        self.enable_vacuum_sucker(True)
        # arm to center position
        g1 = 'G1a0'
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
        self.move_xy_to(row * 40, col * 40)
        self.enable_vacuum_sucker(False)

    def move_z_top(self):
        # print('gcode ','EefMove_Z_toTop')
        g1 = "G1Z0"
        g_amq.Publish(self.exchange_name, self.queue_name, g1)

    def EefMove_Z_toMiddle(self, depth:int):
        g1 = "G1Z30"
        g_amq.Publish(self.exchange_name, self.queue_name, g1)
        # print('gcode ','EefMove_Z_toMiddle')
        # pass

    def Eef_TurnOn_VacuumFan(self, is_turn_on: bool):
        if is_turn_on:
            mcode = 'M42P33S0'
        else:
            mcode = 'M42P33S1'
        g_amq.Publish(self.exchange_name, self.queue_name, mcode)

    def enable_vacuum_sucker(self, is_enable:bool):
        # print('gcode ','Eef_EnableSuck')
        if is_enable:
            m1 = 'M280P0S0'   # servo 0 angle 0
        else:
            m1 = 'M280P0S270'  # servo 0 angle 270
            
        g_amq.Publish(self.exchange_name, self.queue_name, m1)

    def dwell(self, second: int):
        print('gcode ','PauseForWaiting')
        g4 = 'G4S' + str(second)
        g_amq.Publish(self.exchange_name, self.queue_name, g4)

    def move_arm_to_center(self):
        # print('gcode ','MoveArmToCenter')
        self.move_a_to(0)

    def move_a_to(self, angle_in_rad):
        # print('gcode ','MoveTo')
        g1 = "G6a" + str(angle_in_rad) 
        g_amq.Publish(self.exchange_name, self.queue_name, g1)

    def move_xy_to(self, x, y):
        # print('gcode ','MoveTo')
        g1 = "G1X" + str(x) + "Y" + str(y)
        g_amq.Publish(self.exchange_name, self.queue_name, g1)