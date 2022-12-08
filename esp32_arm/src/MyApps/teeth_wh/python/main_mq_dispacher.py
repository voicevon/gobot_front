from twh_robot import TeethWarehouseRobot, UserRequest
from von.amq_agent import g_amq, g_amq_broker_config
from von.mqtt_agent import g_mqtt,g_mqtt_broker_config


class TeethWarehouseMessageDispacher():
    global g_amq

    def __init__(self) -> None:
        self.robots = [TeethWarehouseRobot(221109)]

        # g_amq.Subscribe(queue_name='twh_deposit')
        # g_amq.Subscribe(queue_name='twh_withdraw')

    def __find_robot(self, robot_id:int) -> TeethWarehouseRobot:
        for r in self.robots:
            if r.robot_id == robot_id:
                return r
        return None

    def __deal_deposit(self, user_request: UserRequest) -> bool:
        '''
        return True:  The user_request message is consumed.
        return False: The user_request message is blocked.
        '''
        robot = self.find_robot(user_request.robot_id)
        if robot is None:
            new_robot = TeethWarehouseRobot(user_request.robot_id)
            self.robots.append(new_robot)
            robot = new_robot
  
        is_fed =robot.feed_request(user_request.command)

    def spin_once(self):
        for r in self.robots:
            r.spin_once()
            



if __name__ == '__main__':
    g_amq.connect_to_broker(g_amq_broker_config)
    g_mqtt_broker_config.client_id = '221208'
    g_mqtt.connect_to_broker(g_mqtt_broker_config)
    while not g_mqtt.paho_mqtt_client.is_connected():
        pass
    print("connected to mqtt broker.......")

    twh_message_dealler = TeethWarehouseMessageDispacher()
    while True:
        g_amq.process_data_events()
        twh_message_dealler.spin_once()
