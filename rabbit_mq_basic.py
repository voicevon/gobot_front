import pika 
from pika import BlockingConnection
from von.mqtt_helper import g_mqtt, MQTT_ConnectionConfig

import time

class RabbitMQBrokeConfig:
    host = 'voicevon.vicp.io'
    port = 5672
    virtual_host = '/'
    uid = 'gobot_head'
    password = 'gobot_head'


class RabbitClient():
    '''
    To learn:  What is channel indeed ?
    '''
    def __init__(self, brokerConfig: RabbitMQBrokeConfig) -> None:
        self.broker_config = brokerConfig
        self.connection = self.ConnectToRabbitMq()
        self.channel = self.connection.channel()
        self.declaed_queues=[]

    def ConnectToRabbitMq(self)-> BlockingConnection:
        credentials = pika.PlainCredentials(self.broker_config.uid, self.broker_config.password)
        parameters = pika.ConnectionParameters(host=self.broker_config.host,
                                    port= self.broker_config.port,
                                    virtual_host= self.broker_config.virtual_host,
                                    credentials= credentials)
        connection = pika.BlockingConnection(parameters)
        return connection

    def Publish(self, queue_name:str, payload:str):
        if not (queue_name in self.declaed_queues):
            self.channel.queue_declare(queue=queue_name)
            self.declaed_queues.append(queue_name)

        self.channel.basic_publish(exchange = '',
                        routing_key = queue_name,
                        body = payload)

    def callback_example(self, ch, method, properties, body):
        print('RabbitClient::callback_example()  mq Received ' ,  method.routing_key, body)
        self.channel.basic_ack(delivery_tag=method.delivery_tag)

    def Subscribe(self, queue_name:str, callback):
        '''
        call back examole def callback_main(self, ch, method, properties, body):
        '''
        if not (queue_name in self.declaed_queues):
            self.channel.queue_declare(queue=queue_name)
            self.declaed_queues.append(queue_name)
        if callback is None:
            callback = self.callback_example
        # self.channel_main = self.connection.channel()
        # self.channel_main.queue_declare(queue=self.queue_config.main_queue)
        self.channel.basic_consume(queue=queue_name, on_message_callback=callback, auto_ack=False )

    

    def RabbitMQ_publish_tester(self):
        i = 0
        while True:
            self.channel.basic_publish(exchange='',
                            routing_key='gobot_x2134_house',
                            body = str(i))
            self.channel.basic_publish(exchange='',
                            routing_key='gobot_x2134_arm',
                            body = str(i))
            print(" [x] Sent ",i)
            i += 1  
            time.sleep(2)



def ConnectBroker_MQTT(self):
    config_mqtt = MQTT_ConnectionConfig()
    config_mqtt.uid = 'gobot_head'
    config_mqtt.password = 'gobot_head'
    g_mqtt.connect_to_broker(config_mqtt)