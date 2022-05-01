from math import gamma
import cv2
import numpy
import pika 
from pika import BlockingConnection
from von.mqtt_helper import g_mqtt, MQTT_ConnectionConfig

import time




class AMQ_ConnectionConfig:
    host = 'voicevon.vicp.io'
    port = 5672
    virtual_host = '/'
    uid = 'gobot_head'
    password = 'gobot_head'


class RabbitClient():
    '''
    To learn:  What is channel indeed ?
    '''
    def __init__(self) -> None:
        self.declaed_queues=[]

    def ConnectToRabbitMq(self,serverConfig: AMQ_ConnectionConfig) -> None:
        self.serverConfig = serverConfig
        self.ReconnectToRabbitMq()

    def ReconnectToRabbitMq(self):
        serverConfig = self.serverConfig
        credentials = pika.PlainCredentials(serverConfig.uid, serverConfig.password)
        parameters = pika.ConnectionParameters(host=serverConfig.host,
                                    port= serverConfig.port,
                                    virtual_host= serverConfig.virtual_host,
                                    credentials= credentials)
        try:
            self.blocking_connection = pika.BlockingConnection(parameters)
            self.channel = self.blocking_connection.channel()
        except Exception as e:
            print(e)



    def Publish(self, queue_name:str, payload:str):
        if not (queue_name in self.declaed_queues):
            self.channel.queue_declare(queue=queue_name)
            self.declaed_queues.append(queue_name)

        self.channel.basic_publish(exchange = '',
                        routing_key = queue_name,
                        body = payload)

    def PublishBatch(self, queue_name:str, payloads:list):
        print("[Info] RabbitClient.PublishBatch(), queue_name=, payload= ", queue_name, payloads)
        if not (queue_name in self.declaed_queues):
            self.channel.queue_declare(queue=queue_name)
            self.declaed_queues.append(queue_name)
        for pp in payloads:
            self.channel.basic_publish(exchange = '',
                            routing_key = queue_name,
                            body = pp)

    def publish_cv_image(self, queue_name:str, cv_image):
        # Convert received message into Numpy array
        # jpg = np.frombuffer(RECEIVEDMESSAGE, dtype=np.uint8)

        # # JPEG-decode back into original frame - which is actually a Numpy array
        # im = cv2.imdecode(jpg, cv2.IMREAD_UNCHANGED)
        if not (queue_name in self.declaed_queues):
            self.channel.queue_declare(queue=queue_name)
            self.declaed_queues.append(queue_name)

        is_success, img_encode = cv2.imencode(".jpg", cv_image)
        if is_success:
            img_pub = img_encode.tobytes()
            # self.client.publish(topic, img_pub, retain=retain)
            self.channel.basic_publish(exchange = '',
                        routing_key = queue_name,
                        body = img_pub)

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
    
g_amq = RabbitClient()


if __name__ == '__main__':
    config = AMQ_ConnectionConfig()
    g_amq.ConnectToRabbitMq(config)
    img = cv2.imread("nocommand.jpg")
    g_amq.publish_cv_image("test" , img)