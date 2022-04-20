from pika import BlockingConnection
from von.mqtt_helper import g_mqtt, MQTT_ConnectionConfig
from rabbit_mq_basic import RabbitMQBrokeConfig, RabbitClient
import copy
# from rabbit_mq_basic import Connect

class SyncQueue_MqttTopic:
    def __init__(self, queue_name:str) -> None:
        '''
        standard queue_name = 'gobot_x2134_house'
        feedback_queue = 'gobot_x2134_house_fb'
        main_mqtt_topic = 'gobot/x2134/house'
        '''
        self.main_queue = queue_name
        self.feedback_queue = queue_name + "_fb"
        self.mqtt_publish_topic = queue_name.replace('_', '/')


class RabbitMQSyncer:
    def __init__(self, connection:BlockingConnection, queue_name:str, index:int) -> None:
        self.index = index
        self.main_body = None
        self.feedback_body = None
        self.connection = connection
        self.queues = SyncQueue_MqttTopic(queue_name)
        print('[Info] RabbitMQSyncer.__init__()   feedback_queue_name =', self.queues.feedback_queue)
        self.SubsribeRabbitMQ()
        self.main_delivery_tag = None
        
    def callback_main(self, ch, method, properties, body):
        # print('                       [RabbitMQSyncer] callback_main ' ,  method.routing_key, body)
        if body == self.feedback_body:
            print('repeated...... not return   ' ,body, method.delivery_tag)    #????
            # return

        # a new command from gobot_head is received
        g_mqtt.publish(self.queues.mqtt_publish_topic, body)
        self.main_delivery_tag = method.delivery_tag
        # print("main_delivery_tag= ",self.main_delivery_tag)
        self.main_body = copy.copy(body)   # The main_thread and other thread is sharing self.main_body

    def callback_feedback(self, ch, method, properties, body):
        # print('                             [Info] RabbitMQSyncer.callback_feedback()  ' ,  method.routing_key, body)
        self.feedback_body = body   # The main-thread is sharing self.feedback_body
        if self.main_body == self.feedback_body:
            if self.main_delivery_tag is None:
                print("[Warning] RabbitMQSyncer.callback_feedback()    Only the very begining might happen once.")
                # At the very beginning, This app received a feedback firstly, the main_delivery_ta is None.
                return
            self.channel_main.basic_ack(delivery_tag=self.main_delivery_tag)
            self.main_delivery_tag = None   # Is this necessary?

        else:
            print("[Warn] main != feedback_body", self.main_body,  self.feedback_body)
            pass

 
    def SubsribeRabbitMQ(self):
        self.channel_main = self.connection.channel()
        self.channel_main.basic_qos(prefetch_count=1)
        self.channel_main.queue_declare(queue=self.queues.main_queue)
        self.channel_main.basic_consume(queue=self.queues.main_queue, on_message_callback=self.callback_main, auto_ack=False )

        self.channel_feedback = self.connection.channel()
        self.channel_feedback.basic_qos(prefetch_count=1)
        self.channel_feedback.queue_declare(queue=self.queues.feedback_queue)
        self.channel_feedback.basic_consume(queue=self.queues.feedback_queue, on_message_callback=self.callback_feedback, auto_ack=True )
        # self.channel_main.start_consuming()    // Will always block thread.

    def SpinOnce(self):
        # Only one process_data_events, will cauase all callback invoked.  !!!
        # Not involved to which channel !!!
        # if self.channel_main._consumer_infos:
        if self.index !=0:
            print("[Warn] RabbitMQSyncer.SpinOnce(), Skip call me is better,  channel_main ", self.queues.main_queue)
        self.channel_main.connection.process_data_events(time_limit=0.001)  # will blocking 0.1 second

        # if self.channel_feedback._consumer_infos:
        #     print("[Info] RabbitMQSyncer.SpinOnce() :: channel_feedback ", self.queues.feedback_queue)
        #     self.channel_feedback.connection.process_data_events(time_limit=0.1)
        pass


class SyncerHelper:
    def __init__(self, connection:BlockingConnection ) -> None:
        self.connection = connection
        self.all_syncers=[]
        
    def MakeSyncer(self, main_queue_name:str) -> None:
        index = len(self.all_syncers)
        sync = RabbitMQSyncer(self.connection, main_queue_name, index)
        self.all_syncers.append(sync)
    
    @staticmethod
    def ConnectMqttBroker(config:MQTT_ConnectionConfig):
        g_mqtt.connect_to_broker(config)


    def SpinOnce(self) -> None:
        for syncer in self.all_syncers:
            # print(syncer.queues.main_queue)
            syncer.SpinOnce()
            return   # Only the first syncer runs, all other syncers should invoke callback.

# TODO:  remove this
class SyncerHelper_ForGobot:
    def __init__(self) -> None:
        config_mqtt = MQTT_ConnectionConfig()
        config_mqtt.uid = 'agent'
        config_mqtt.password = 'agent'


        config_rabbit = RabbitMQBrokeConfig()
        config_rabbit.uid = 'agent'
        config_rabbit.password = 'agent'
        self.MqClient = RabbitClient(config_rabbit)
        self.MqConnection = self.MqClient.connection

        self.helper = SyncerHelper(self.MqConnection)
        self.helper.ConnectMqttBroker(config_mqtt)
        self.helper.MakeSyncer('gobot_x2134_house')
        self.helper.MakeSyncer('gobot_x2134_arm')

    def SpinOnce(self):
        self.helper.SpinOnce()

if __name__ == '__main__':
    app = SyncerHelper_ForGobot()
    while True:
        app.SpinOnce()
