from asyncio.log import logger
import pika 

import time
from rabbitmq_mqtt_sync import RabbitMQSync, SyncQueueTopicConfig


class RabbitMQBrokeConfig:
    host = 'voicevon.vicp.io'
    port = 5672
    virtual_host = '/'
    uid = 'gobot_head'
    password = 'gobot_head'


class RabbitClient():
    def __init__(self, connection:pika.BlockingConnection) -> None:
        self.connection = connection
        self.MakeChannel()

    def MakeChannel(self):
        self.channel = self.connection.channel()
        self.channel.queue_declare(queue='gobot_x2134_house')
        self.channel.queue_declare(queue='gobot_x2134_arm')
        self.channel.queue_declare(queue='agv_x2206')

    def PublishToArm(self, gcode:str):
        self.channel.basic_publish(exchange='',
                        routing_key='gobot_x2134_arm',
                        body = gcode)

    def PublishToHouse(self, gcode:str):
        self.channel.basic_publish(exchange='',
                        routing_key='gobot_x2134_house',
                        body = gcode)

    def PublishToAgv(self, routing_key:str, gcode:str):
        self.channel.basic_publish(exchange='',
                        # routing_key='agv_x2206',
                        routing_key=routing_key,
                        body = gcode)

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



class SyncConfigFactory:

    @staticmethod
    def MakeHouseConfig() -> SyncQueueTopicConfig:
        config = SyncQueueTopicConfig()
        config.main_mqtt_topic = 'gobot/x2134/house'
        config.main_queue = "gobot_x2134_house"
        config.feedback_queue = 'gobot_x2134_house_fb'
        return config

    @staticmethod
    def MakeArmConfig() -> SyncQueueTopicConfig:
        config = SyncQueueTopicConfig()
        config.main_mqtt_topic = 'gobot/x2134/arm'
        config.main_queue = "gobot_x2134_arm"
        config.feedback_queue = 'gobot_x2134_arm_fb'
        return config

    @staticmethod
    def MakeAgvConfig() -> SyncQueueTopicConfig:
        config = SyncQueueTopicConfig()
        config.main_mqtt_topic = 'agv/x2206'
        config.main_queue = "agv_x2206"
        config.feedback_queue = 'agv_x2206_fb'
        return config

class RabbitMqClient_Helper():
    '''
    top level interface  of MQTT and RabbitMQ

    '''
    def __init__(self) -> None:
        self.__ConnectBroker_MQTT()
        self.__ConnectBroker_RabbitMQ()
        self.__MakeSyncer()

    def SpinOnce(self):
        self.arm_sync.SpinOnce()
        self.house_sync.SpinOnce()
        self.agv_sync.SpinOnce()

    def __MakeSyncer(self):
        config = SyncConfigFactory.MakeHouseConfig()
        self.house_sync = RabbitMQSync(self.connection, config=config)

        config = SyncConfigFactory.MakeArmConfig()
        self.arm_sync = RabbitMQSync(self.connection, config=config)

        config = SyncConfigFactory.MakeAgvConfig()
        self.agv_sync = RabbitMQSync(self.connection, config=config)

    def MakeClient(self)->RabbitClient:
        self.client = RabbitClient(self.connection)
        return self.client

    def __ConnectBroker_MQTT(self):
        from von.mqtt_helper import g_mqtt, MQTT_ConnectionConfig
        config_mqtt = MQTT_ConnectionConfig()
        config_mqtt.uid = 'gobot_head'
        config_mqtt.password = 'gobot_head'
        g_mqtt.connect_to_broker(config_mqtt)

    def __ConnectBroker_RabbitMQ(self):
        config = RabbitMQBrokeConfig()
        config.uid = 'gobot_head'
        config.password = 'gobot_head'
        credentials = pika.PlainCredentials(config.uid, config.password)
        parameters = pika.ConnectionParameters(host=config.host,
                                        port= config.port,
                                        virtual_host= config.virtual_host,
                                        credentials= credentials)
        self.connection = pika.BlockingConnection(parameters)


if __name__ == '__main__':
    helper = RabbitMqClient_Helper()
    client = helper.MakeClient()
    i=0
    while True:
        helper.SpinOnce()
        helper.SpinOnce()
        helper.SpinOnce()


        # gcode = 'G1X0'
        # client.PublishToHouse(gcode=gcode)
        # client.PublishToArm(gcode=gcode)
        # gcode = 'G1X100'
        # client.PublishToHouse(gcode=gcode)
        # client.PublishToArm(gcode=gcode)
        # time.sleep(0.5)

