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
        self.a = None
        self.b = None

        '''
        Only for goboy_x2134_house
        '''
        self.consuming_message_in_queue = True


    def callback(self, ch, method, properties, body):
        print('[x] Received ' ,  method.routing_key, body)
        if method.routing_key == 'gobot_x2134_house':
            self.a = body
            # a new command from gobot_head is received
            g_mqtt.publish('gobot/x2134/house/', body)
            #stop consume a
            print("Stop consuming now..")
            # self.channel.stop_consuming()
            self.consuming_message_in_queue = False



        if method.routing_key == 'gobot.x2134.house.fb':
            self.b = body
            if self.a == self.b:
                # feedback is equal to last command.
                # gobot-house has received last message in the gobot_head command queue.
                # go on to comsume a
                self.consuming_message_in_queue = True
                print("Start consuming now..")

    def SubsribeRabbitMQ(self, config:RabbitMQConfig):
        credentials = pika.PlainCredentials(config.uid, config.password)
        parameters = pika.ConnectionParameters(host=config.host,
                                        port= config.port,
                                        virtual_host= config.virtual_host,
                                        credentials= credentials)
        self.connection = pika.BlockingConnection(parameters)
        self.channel = self.connection.channel()
        self.channel.queue_declare(queue='gobot_x2134_house')
        self.channel.basic_consume(queue='gobot_x2134_house', on_message_callback=self.callback, auto_ack=True )
        self.channel.queue_declare(queue='gobot_x2134_house_fb')
        self.channel.basic_consume(queue='gobot_x2134_house_fb', on_message_callback=self.callback, auto_ack=True )
        # channel.start_consuming()
        while True:
            if self.consuming_message_in_queue:
                if self.channel._consumer_infos:
                    self.channel.connection.process_data_events(time_limit=1)
            # print(".")

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