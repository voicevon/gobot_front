import pika 
from rabbit_mq_basic import RabbitMQBrokeConfig,RabbitClient
from rabbitmq_mqtt_sync import RabbitMQSync





class RabbitMqClient_App_Example():
    '''
    top level interface  of MQTT and RabbitMQ

    '''
    def __init__(self) -> None:
        self.__ConnectBroker_MQTT()
        self.__MakeSyncer()

    def SpinOnce(self):
        self.arm_sync.SpinOnce()
        self.house_sync.SpinOnce()
        self.agv_sync.SpinOnce()

    def __MakeSyncer(self):
        self.house_sync = RabbitMQSync(self.connection, 'gobot_x2134_house')
        self.arm_sync = RabbitMQSync(self.connection, 'gobot_x2134_arm')
        self.agv_sync = RabbitMQSync(self.connection, 'puma_agv_x226')


    # def MakeClient(self)->RabbitClient:
    #     self.client = RabbitClient(self.connection)
    #     return self.client



    # def __ConnectBroker_RabbitMQ(self):
    #     config = RabbitMQBrokeConfig()
    #     config.uid = 'gobot_head'
    #     config.password = 'gobot_head'
    #     credentials = pika.PlainCredentials(config.uid, config.password)
    #     parameters = pika.ConnectionParameters(host=config.host,
    #                                     port= config.port,
    #                                     virtual_host= config.virtual_host,
    #                                     credentials= credentials)
    #     self.connection = pika.BlockingConnection(parameters)


if __name__ == '__main__':
    app = RabbitMqClient_App_Example()
    client = app.MakeClient()
    i=0
    while True:
        app.SpinOnce()


        # gcode = 'G1X0'
        # client.PublishToHouse(gcode=gcode)
        # client.PublishToArm(gcode=gcode)
        # gcode = 'G1X100'
        # client.PublishToHouse(gcode=gcode)
        # client.PublishToArm(gcode=gcode)
        # time.sleep(0.5)

