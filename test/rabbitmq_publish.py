import pika 
import time

class RabbitMQConfig:
    host = 'voicevon.vicp.io'
    port = 5672
    virtual_host = '/'
    uid = ''
    password = ''



def test_RabbitMQ(config:RabbitMQConfig):
    credentials = pika.PlainCredentials(config.uid, config.password)
    parameters = pika.ConnectionParameters(host=config.host,
                                       port= config.port,
                                       virtual_host= config.virtual_host,
                                       credentials= credentials)
    connection = pika.BlockingConnection(parameters)
    channel = connection.channel()
    channel.queue_declare(queue='gobot_x2134_house')

    i = 0
    while True:
        channel.basic_publish(exchange='',
                        routing_key='gobot_x2134_house',
                        body = str(i))
        print(" [x] Sent ",i)
        i += 1  
        time.sleep(2)


if __name__ == '__main__':
    config_rabbit = RabbitMQConfig()
    config_rabbit.uid = 'gobot_head'
    config_rabbit.password = 'gobot_head'
    test_RabbitMQ(config_rabbit)