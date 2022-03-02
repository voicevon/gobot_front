import pika 
from pika import BlockingConnection
from von.mqtt_helper import g_mqtt,MQTT_ConnectionConfig

class RabbitMQConfig:
    host = 'voicevon.vicp.io'
    port = 5672
    virtual_host = '/'
    uid = ''
    password = ''

class AgentQueueTopicConfig:
    main_queue = 'gobot_x2134_house'
    main_mqtt_topic = 'gobot/x2134/house'
    feedback_queue = 'gobot_x2134_house_fb'


class RabbitMQAgent:

    def __init__(self, config:AgentQueueTopicConfig, connection:BlockingConnection) -> None:
        self.main = None
        self.feedback = None
        self.connection = connection
        self.message_config = config

        '''
        Only for goboy_x2134_house
        '''
        self.consuming_message_in_queue = True
        


    def callback_main(self, ch, method, properties, body):
        print('                        [main] Received ' ,  method.routing_key, body)
        # if method.routing_key == 'gobot_x2134_house':
        self.main = body
        if self.main == self.feedback:
            print('repeated...... return')
            return

        # a new command from gobot_head is received
        g_mqtt.publish(self.message_config.main_mqtt_topic, body)
        self.channel_main.basic_ack(delivery_tag=method.delivery_tag)
        #stop consume a
        print("                       Stop consuming now..", self.message_config.main_mqtt_topic, body)
        self.channel_main.stop_consuming()  # this will break all callbacks
        self.consuming_message_in_queue = False
        self.channel_feedback.queue_declare(queue=self.message_config.feedback_queue)
        self.channel_feedback.basic_consume(queue=self.message_config.feedback_queue, on_message_callback=self.callback_feedback, auto_ack=True )

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
            self.channel_main.queue_declare(queue=self.message_config.main_queue)
            self.channel_main.basic_consume(queue=self.message_config.main_queue, on_message_callback=self.callback_main, auto_ack=False )
 


    def SubsribeRabbitMQ(self, config:RabbitMQConfig):


        self.channel_main = self.connection.channel()
        self.channel_main.queue_declare(queue=self.message_config.main_queue)
        self.channel_main.basic_consume(queue=self.message_config.main_queue, on_message_callback=self.callback_main, auto_ack=False )

        self.channel_feedback =  self.connection.channel()
        self.channel_feedback.queue_declare(queue=self.message_config.feedback_queue)
        self.channel_feedback.basic_consume(queue=self.message_config.feedback_queue, on_message_callback=self.callback_feedback, auto_ack=True )
        # self.channel_main.start_consuming()  

    def SpinOnce(self):
        # if self.consuming_message_in_queue:
        if self.channel_main._consumer_infos:
            self.channel_main.connection.process_data_events(time_limit=1)
                # sprint(".")
        if self.channel_feedback._consumer_infos:
            self.channel_feedback.connection.process_data_events(time_limit=1)
        # while True:
        #     pass

def Connect(config:RabbitMQConfig) -> BlockingConnection:
    credentials = pika.PlainCredentials(config.uid, config.password)
    parameters = pika.ConnectionParameters(host=config.host,
                                    port= config.port,
                                    virtual_host= config.virtual_host,
                                    credentials= credentials)
    connection = pika.BlockingConnection(parameters)
    return connection

if __name__ == '__main__':
    config_mqtt = MQTT_ConnectionConfig()
    config_mqtt.uid = 'agent'
    config_mqtt.password = 'agent'
    g_mqtt.connect_to_broker(config_mqtt)

    config_rabbit = RabbitMQConfig()
    config_rabbit.uid = 'agent'
    config_rabbit.password = 'agent'
    connection = Connect(config_rabbit)


    config_house = AgentQueueTopicConfig()
    config_house.main_mqtt_topic = 'gobot/x2134/house'
    config_house.main_queue = "gobot_x2134_house"
    config_house.feedback_queue = 'gobot_x2134_house_fb'

    config_arm = AgentQueueTopicConfig()
    config_arm.main_mqtt_topic = 'gobot/x2134/arm'
    config_arm.main_queue = "gobot_x2134_arm"
    config_arm.feedback_queue = 'gobot_x2134_arm_fb'


    runner_house = RabbitMQAgent(config_house,connection)
    runner_house.SubsribeRabbitMQ(config_rabbit)
    
    runner_arm = RabbitMQAgent(config_arm,connection)
    runner_arm.SubsribeRabbitMQ(config_rabbit)


    while True:
        runner_house.SpinOnce()
        runner_arm.SpinOnce()
