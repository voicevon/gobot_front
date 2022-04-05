import queue
from pika import BlockingConnection
from von.mqtt_helper import g_mqtt, MQTT_ConnectionConfig
from rabbit_mq_basic import RabbitMQBrokeConfig
from rabbit_mq_basic import Connect

class SyncQueueTopicConfig:
    def __init__(self, queue_name:str) -> None:
        '''
        standard queue_name = 'gobot_x2134_house'
        feedback_queue = 'gobot_x2134_house_fb'
        main_mqtt_topic = 'gobot/x2134/house'
        '''
        self.main_queue = queue_name
        self.feedback_queue = queue_name + "_fb"
        self.main_mqtt_topic =queue_name.replace('_', '/')


class RabbitMQSync:
    def __init__(self, connection:BlockingConnection, queue_name:str) -> None:
        self.main = None
        self.feedback = None
        self.connection = connection
        self.queue_config = SyncQueueTopicConfig(queue_name)
        self.SubsribeRabbitMQ()
        self.consuming_message_in_queue = True
        


    def callback_main(self, ch, method, properties, body):
        print('                        [main] Received ' ,  method.routing_key, body)
        # if method.routing_key == 'gobot_x2134_house':
        self.main = body
        if self.main == self.feedback:
            print('repeated...... return')    #????
            # return

        # a new command from gobot_head is received
        g_mqtt.publish(self.message_config.main_mqtt_topic, body)
        self.channel_main.basic_ack(delivery_tag=method.delivery_tag)
        #stop consume a
        print("                       Stop consuming now..", self.queue_config.main_mqtt_topic, body)
        self.channel_main.stop_consuming()  # this will break all callbacks
        self.consuming_message_in_queue = False
        self.channel_feedback.queue_declare(queue=self.queue_config.feedback_queue)
        self.channel_feedback.basic_consume(queue=self.queue_config.feedback_queue, on_message_callback=self.callback_feedback, auto_ack=True )

    def callback_feedback(self, ch, method, properties, body):
        # if method.routing_key == 'gobot.x2134.house.fb':
        print('[feedback] Received ' ,  method.routing_key, body)
        self.feedback = body
        if self.main == self.feedback:
            # feedback is equal to last command.
            # gobot-house has received last message in the gobot_head command queue.
            # go on to comsume a
            self.consuming_message_in_queue = True
            print(self.main, self.feedback, "Start consuming now..")
            self.channel_main.queue_declare(queue=self.queue_config.main_queue)
            self.channel_main.basic_consume(queue=self.queue_config.main_queue, on_message_callback=self.callback_main, auto_ack=False )
 
    def SubsribeRabbitMQ(self):
        self.channel_main = self.connection.channel()
        self.channel_main.queue_declare(queue=self.queue_config.main_queue)
        self.channel_main.basic_consume(queue=self.queue_config.main_queue, on_message_callback=self.callback_main, auto_ack=False )

        self.channel_feedback =  self.connection.channel()
        self.channel_feedback.queue_declare(queue=self.queue_config.feedback_queue)
        self.channel_feedback.basic_consume(queue=self.queue_config.feedback_queue, on_message_callback=self.callback_feedback, auto_ack=True )
        # self.channel_main.start_consuming()  

    def SpinOnce(self):
        # if self.consuming_message_in_queue:
        if self.channel_main._consumer_infos:
            self.channel_main.connection.process_data_events(time_limit=0.1)  # will blocking 0.1 second

        if self.channel_feedback._consumer_infos:
            self.channel_feedback.connection.process_data_events(time_limit=0.1)



if __name__ == '__main__':
    config_mqtt = MQTT_ConnectionConfig()
    config_mqtt.uid = 'agent'
    config_mqtt.password = 'agent'
    g_mqtt.connect_to_broker(config_mqtt)

    config_rabbit = RabbitMQBrokeConfig()
    config_rabbit.uid = 'agent'
    config_rabbit.password = 'agent'
    connection = Connect(config_rabbit)

    runner_house = RabbitMQSync(connection,'gobot_x2134_house')
    runner_arm = RabbitMQSync(connection, 'gobot_x2134_arm')

    while True:
        runner_house.SpinOnce()
        runner_arm.SpinOnce()
