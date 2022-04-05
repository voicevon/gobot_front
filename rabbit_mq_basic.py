import pika 
from pika import BlockingConnection

import time

class RabbitMQBrokeConfig:
    host = 'voicevon.vicp.io'
    port = 5672
    virtual_host = '/'
    uid = 'gobot_head'
    password = 'gobot_head'

class RabbitClient():
    # def __init__(self, connection:pika.BlockingConnection) -> None:
    def __init__(self, brokerConfig: RabbitMQBrokeConfig) -> None:
        self.connection = self.ConnectToRabbitMq(brokerConfig)
        self.channel = self.connection.channel()
        self.declaed_queues=[]

    def ConnectToRabbitMq(self, config:RabbitMQBrokeConfig )-> BlockingConnection:
        credentials = pika.PlainCredentials(config.uid, config.password)
        parameters = pika.ConnectionParameters(host=config.host,
                                    port= config.port,
                                    virtual_host= config.virtual_host,
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

    def Subscribe(self):
        pass
    

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

# def Connect(config:RabbitMQBrokeConfig) -> BlockingConnection:
#     credentials = pika.PlainCredentials(config.uid, config.password)
#     parameters = pika.ConnectionParameters(host=config.host,
#                                     port= config.port,
#                                     virtual_host= config.virtual_host,
#                                     credentials= credentials)
#     connection = pika.BlockingConnection(parameters)
#     return connection