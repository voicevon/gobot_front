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
    def __init__(self, config:RabbitMQBrokeConfig) -> None:
        self.config = config
        self.ConnectToBroker()

        config = SyncConfigFactory.MakeHouseConfig()
        self.house_sync = RabbitMQSync(self.connection, config=config)

        config = SyncConfigFactory.MakeArmConfig()
        self.arm_sync = RabbitMQSync(self.connection, config=config)

    def ConnectToBroker(self):
        credentials = pika.PlainCredentials(self.config.uid, self.config.password)
        parameters = pika.ConnectionParameters(host=self.config.host,
                                        port= self.config.port,
                                        virtual_host= self.config.virtual_host,
                                        credentials= credentials)
        self.connection = pika.BlockingConnection(parameters)
        self.channel = self.connection.channel()
        self.channel.queue_declare(queue='gobot_x2134_house')
        self.channel.queue_declare(queue='gobot_x2134_arm')

    def PublishToArm(self, gcode:str):
        self.channel.basic_publish(exchange='',
                        routing_key='gobot_x2134_arm',
                        body = gcode)

    def PublishToHouse(self, gcode:str):
        self.channel.basic_publish(exchange='',
                        routing_key='gobot_x2134_house',
                        body = gcode)

    def SpinOnce(self):
        self.arm_sync.SpinOnce()
        self.house_sync.SpinOnce()

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


class RabbitMqClient_Helper():

    @staticmethod
    def MakeClient()->RabbitClient:
        from von.mqtt_helper import g_mqtt, MQTT_ConnectionConfig
        config_mqtt = MQTT_ConnectionConfig()
        config_mqtt.uid = 'gobot_head'
        config_mqtt.password = 'gobot_head'
        g_mqtt.connect_to_broker(config_mqtt)

        config = RabbitMQBrokeConfig()
        config.uid = 'gobot_head'
        config.password = 'gobot_head'
        client = RabbitClient(config)
        client.SpinOnce()
        return client


if __name__ == '__main__':
    client = RabbitMqClient_Helper.MakeClient()
    i=0
    while True:
        gcode = 'G1X' + str(i)
        client.PublishToHouse(gcode=gcode)
        print(gcode)
        i+=1
        # time.sleep(0.5)
        client.SpinOnce()
