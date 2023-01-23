# from von.amq_agent import g_amq, g_amq_broker_config
from twh_robot_row import TwhRobot_Row
from twh_robot_shipout import TwhRobot_ShipoutBox, TwhRobot_Shipout
from database.db_api import g_database

import requests
import json


class WithdrawOrderTooth():
    def __init__(self, row:int, col:int, layer:int) -> None:
        self.row =  row
        self.col = col
        self.layer = layer


class WithdrawOrder():
    def __init__(self, order_id:str, teeth)-> None:
        self.order_id = order_id
        self.teeth = [WithdrawOrderTooth(0,0,0)]
        self.teeth = teeth
    
    def connect_shipoutbox(self, box: TwhRobot_ShipoutBox):
        self.__connected_shipout_box = box

        g_database.update_withdraw_queue_multi_rows_connected_shipout_box(self.order_id, box.id)
    
    def RemoveTooth(self, tooth):
        self.teeth.remove(tooth)
        if len(self.teeth) == 0:
            self.__connected_shipout_box.state = 'fullfilled'

    def FindTooth_FromRow(self, row: int) -> WithdrawOrderTooth:
        for tooth in self.teeth:
            if tooth.row == row:
                return tooth
        return None

class TwhRobot_v4():
    def __init__(self) -> None:
        # g_amq.Subscribe('twh_deposit')
        # g_amq.Subscribe('twh_withdraw')
        # self.layer_robots = [TwhRobot_Layer(robot_id='221109')]
        self.robot_rows = [TwhRobot_Row()]
        self.robot_rows.clear()
        for i in range(4):
            new_robot_row = TwhRobot_Row()
            self.robot_rows.append(new_robot_row)
        self.robot_shipout = TwhRobot_Shipout()

        # self.current_state = 'idle'
        self.withdraw_orders = [WithdrawOrder('',0,0,0)]
        self.withdraw_orders.clear()

    
    def AppendWithdrawOrder(self, order:WithdrawOrder)->None:
        self.withdraw_orders.append(order)

    def FindRobotRow_idle(self) -> TwhRobot_Row:
        for robot in self.robot_rows:
            if robot.state == 'idle':
                return robot
        return None

    def assign_shipoutbox_to_order(self):
        shipout_box = self.robot_shipout.FindBox_Idle()
        if shipout_box is None:
            # no idle box
            return

        for order in self.withdraw_orders:
            if order.connected_shipoutbox is None:
                # new order, have found idle shipout_box
                order.connect_shipoutbox(shipout_box)
                shipout_box.state = 'feeding'
                # g_mqtt.publish("box", '"box_id":' + str(shipout_box.id))
                return

    def SpinOnce(self) -> None:
        self.assign_shipoutbox_to_order()
        for row_robot in self.robot_rows:
            if not row_robot.is_moving:
                # Got an idle row_robot
                for order in self.withdraw_orders:
                    tooth = order.FindTooth_FromRow(row_robot.row_id)
                    if  tooth is not None:
                        # continue to feed order to connected shipout box
                        row_robot.move_to(tooth.col, tooth.layer)
                        order.RemoveTooth(tooth)
                        if len(order.teeth) == 0:
                            order.connected_shipoutbox.state = 'fullfilled'
                        return





if __name__ == '__main__':
    from von.mqtt_agent import g_mqtt, g_mqtt_broker_config
    g_mqtt_broker_config.client_id = '221223'
    g_mqtt.connect_to_broker(g_mqtt_broker_config)
    # g_amq.connect_to_broker(g_amq_broker_config)


    robot = TwhRobot_v4()
    while True:
        # g_amq.process_data_events()
        robot.state_machine_spin_once()