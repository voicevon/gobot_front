import pika 
import time
from von.mqtt_helper import g_mqtt,MQTT_ConnectionConfig

class RabbitMQConfig:
    host = 'voicevon.vicp.io'
    port = 5672
    virtual_host = '/'
    uid = ''
    password = ''


class RabbitMQAgent:

    def __init__(self) -> None:
        self.main = None
        self.feedback = None

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
        g_mqtt.publish('gobot/x2134/house/', body)
        self.channel_main.basic_ack(delivery_tag=method.delivery_tag)
        #stop consume a
        print("                       Stop consuming now..")
        self.channel_main.stop_consuming()  # this will break all callbacks
        self.consuming_message_in_queue = False
        self.channel_feedback.queue_declare(queue='gobot_x2134_house_fb')
        self.channel_feedback.basic_consume(queue='gobot_x2134_house_fb', on_message_callback=self.callback_feedback, auto_ack=True )


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
            self.channel_main.queue_declare(queue='gobot_x2134_house')
            self.channel_main.basic_consume(queue='gobot_x2134_house', on_message_callback=self.callback_main, auto_ack=False )
 
    def SubsribeRabbitMQ(self, config:RabbitMQConfig):
        credentials = pika.PlainCredentials(config.uid, config.password)
        parameters = pika.ConnectionParameters(host=config.host,
                                        port= config.port,
                                        virtual_host= config.virtual_host,
                                        credentials= credentials)
        self.connection = pika.BlockingConnection(parameters)
        self.channel_main = self.connection.channel()
        self.channel_main.queue_declare(queue='gobot_x2134_house')
        self.channel_main.basic_consume(queue='gobot_x2134_house', on_message_callback=self.callback_main, auto_ack=False )
        self.channel_feedback =  self.connection.channel()
        self.channel_feedback.queue_declare(queue='gobot_x2134_house_fb')
        self.channel_feedback.basic_consume(queue='gobot_x2134_house_fb', on_message_callback=self.callback_feedback, auto_ack=True )
        # self.channel_main.start_consuming()  

        while True:
            # if self.consuming_message_in_queue:
            if self.channel_main._consumer_infos:
                self.channel_main.connection.process_data_events(time_limit=1)
                    # sprint(".")
            if self.channel_feedback._consumer_infos:
                self.channel_feedback.connection.process_data_events(time_limit=1)
        while True:
            pass

if __name__ == '__main__':
    config_mqtt = MQTT_ConnectionConfig()
    config_mqtt.uid = 'agent'
    config_mqtt.password = 'agent'
    g_mqtt.connect_to_broker(config_mqtt)

    config_rabbit = RabbitMQConfig()
    config_rabbit.uid = 'agent'
    config_rabbit.password = 'agent'

    runner = RabbitMQAgent()
    runner.SubsribeRabbitMQ(config_rabbit)