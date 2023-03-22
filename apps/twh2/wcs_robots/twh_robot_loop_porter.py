from von.remote_var_mqtt import RemoteVar_mqtt
from wcs_robots.gcode_sender import GcodeSender, all_gcode_senders
# from bolt_nut import PickingPacking_Tooth
from business_logical.withdraw_order import OrderTooth, WithdrawOrder


class TwhRobot_LoopPorter():

    def __init__(self, robot_id:str, row_id:int) -> None:
        self.id = row_id
        self.robot_id = robot_id
        self.__state_topic = "twh/" + robot_id + '/r' + str(row_id) + "/state"  #'twh/221109/r0/state'
        self.__state = RemoteVar_mqtt(self.__state_topic, 'idle')

        gcode_topic = "twh/" + robot_id + '/r' + str(row_id) + "/gcode"  #'twh/221109/r0/state'
        self.gcode_sender = GcodeSender(gcode_topic)
        all_gcode_senders.append(self.gcode_sender)

        # self.__porting_tooth = None
        self.__target_layer:int

        # self.IsNone = False
        # if row_id == -1:
        #     self.IsNone = True

    def Start_Porting(self, tooth:OrderTooth, order:WithdrawOrder):
        self.__porting_tooth = tooth
        self.__porting_order = order
        self.MoveTo(tooth.col, tooth.layer)

    def GetPortingTooth(self) -> tuple[OrderTooth, WithdrawOrder]:
        return self.__porting_tooth, self.__porting_order
    

    def SetStateTo(self, new_state:str):
        self.__state.set(new_state)
    
    def GetState(self) -> str:
        return self.__state.get()

    # def port_to_pick(self, tooth: PickingPacking_Tooth) -> None:
    def MoveTo(self, target_col:int, target_layer:int) -> None:
        self.__state.set('moving')    # set to 'moving' when gcode-G1 is sent. ??
        # self.target_tooth = tooth
        self.__target_layer = target_layer
        print("TwhRobot_Row::move_to()", 'row, col, layer = ' ,self.id, target_col, target_layer )
        
        mcode ='M42P99S1'  # turn off all green leds
        self.gcode_sender.append_gmcode_to_queue(mcode)

        gcode = 'G1X' + str(target_col)
        self.gcode_sender.append_gmcode_to_queue(gcode)

        mcode = 'M408' + self.__state_topic
        self.gcode_sender.append_gmcode_to_queue(mcode)

        mcode ='M999'
        self.gcode_sender.append_gmcode_to_queue(mcode)

    def show_layer_led(self):
        mcode = 'M42P' + str(self.__target_layer) + 'S1'
        self.gcode_sender.append_gmcode_to_queue(mcode)

        mcode ='M999'
        self.gcode_sender.append_gmcode_to_queue(mcode)

    def turn_off_leds(self):
        mcode = 'M42P99S1'
        self.gcode_sender.append_gmcode_to_queue(mcode)

        mcode ='M999'
        self.gcode_sender.append_gmcode_to_queue(mcode)  

