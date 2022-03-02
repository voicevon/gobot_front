import pika 
import time

class RabbitMQBrokeConfig:
    host = 'voicevon.vicp.io'
    port = 5672
    virtual_host = '/'
    uid = ''
    password = ''


class RabbitClient():
    def __init__(self, config:RabbitMQBrokeConfig) -> None:
        self.config = config
        self.ConnectToBroker()

    def ConnectToBroker(self):
        credentials = pika.PlainCredentials(config.uid, config.password)
        parameters = pika.ConnectionParameters(host=config.host,
                                        port= config.port,
                                        virtual_host= config.virtual_host,
                                        credentials= credentials)
        connection = pika.BlockingConnection(parameters)
        self.channel = connection.channel()
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


if __name__ == '__main__':
    config = RabbitMQBrokeConfig()
    config.uid = 'gobot_head'
    config.password = 'gobot_head'
    client = RabbitClient(config)
    # client.RabbitMQ_publish_tester()
    i=0
    while True:
        gcode = 'G1X' + str(i)
        client.PublishToHouse(gcode=gcode)
        print(gcode)
        i+=1
        time.sleep(0.5)