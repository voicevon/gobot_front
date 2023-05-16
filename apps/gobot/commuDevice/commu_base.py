
# TODO:  move these code to vonPylib
from enum import Enum
from abc import ABC, abstractmethod

class CommuChannel(Enum):
    RABBIT_MQ = 1
    MQTT = 11
    TCP_SOCKET = 12
    BLE = 13
    LINUX_ESP_NOW = 14



class CommuDevice(ABC):
    def __init__(self, channel:CommuChannel, channel_config) -> None:
        self.channel = channel
        if channel == CommuChannel.MQTT:
            from von.mqtt_helper import g_mqtt
            g_mqtt.connect_to_broker(channel_config) 

        if channel == CommuChannel.TCP_SOCKET:
            pass

        if channel == CommuChannel.BLE:
            pass

    @abstractmethod
    def publish(self, data):
        pass

class CommuRabbitMQ(CommuDevice):
    # python -m pip install pika --upgrade
    def publish(self, data):
        return super().publish(data)

from von.mqtt_helper import g_mqtt,MQTT_ConnectionConfig
class CommuMQTT(CommuDevice):

    def publish(self, topic, message):
        g_mqtt.publish('gobot/node_1234/arm','{x1:1,y1:2,x2:1,y2:1}')        

    def publish_robot_command(self, position_pickup_from, position_place_to):
        pass


import pika
import time
def test_RabbitMQ():
    credentials = pika.PlainCredentials('von', 'von1970')
    parameters = pika.ConnectionParameters(host='voicevon.vicp.io',
                                       port= 5672,
                                       virtual_host= '/',
                                       credentials= credentials)
    connection = pika.BlockingConnection(parameters)
    channel = connection.channel()
    channel.queue_declare(queue='hello')
    i = 0
    while True:
        channel.basic_publish(exchange='',
                        routing_key='hello',
                        # body='Hello World!')
                        body = str(i))
        print(" [x] Sent 'Hello World!'")  
        time.sleep(2)
    # connection.close()

if __name__ == '__main__':

    # import socket
    # socket.getaddrinfo('voicevon.vicp.io', 5672)

    test_RabbitMQ()
    if False:
        config = MQTT_ConnectionConfig()
        config.client_id = '220225'
        config.uid = 'von'
        config.password = 'von1970'
        # runner = CommuDevice(CommuChannel.MQTT, mqtt_config)
        runner = CommuMQTT(CommuChannel.MQTT, config)
        runner.publish("test","test")
        while True:
            pass
