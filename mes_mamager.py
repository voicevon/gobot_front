import json
from tokenize import Single
from robot_tasks_scheduler.bot_tasks_queue import TaskQueue

from rabbitmq_app_examle import RabbitClient, RabbitMqClient_Helper
from rabbitmq_mqtt_sync import Connect

from AGV.mes_resources import MapElement_AGV,MapElement_Node, MES_Resources, MyJsonEncoder

class Single_MesTask:
    def __init__(self) -> None:
        self.load_from = MapElement_Node()
        self.unload_to = MapElement_Node()



class MesManager:
    '''
    Receive message from queue topic='puma/mes',
        The message include LoadFromStation, UnloadToStation
    publish message to queue topic ='puma/agv/v123' \n
    The agv will dispatch loading/unloading to box_mover.
    '''
    def __init__(self) -> None:
        '''
        First, We initialize our resources: 
        1. Map:  RoadNode, branch-side.
        2. Workstations. Where can do loading, unloading, charging, parking.
        3. Agvs. All Agvs we can schedule.
        '''
        self.mq_top_level = TaskQueue()
        self.mes_resources = MES_Resources()
        helper = RabbitMqClient_Helper()
        self.mq_client = helper.MakeClient()
        self.mq_client.Publish()
        self.mq_rx_channel = self.connection.channel()
        self.mq_rx_channel.queue_declare(queue=self.message_config.main_queue)
        self.mq_rx_channel.basic_consume(queue=self.message_config.main_queue, on_message_callback=self.callback_main, auto_ack=False )

    def callback_main(self, ch, method, properties, body):
        print('                        [main] Received ' ,  method.routing_key, body)
        # if method.routing_key == 'gobot_x2134_house':
        self.main = body
        if self.main == self.feedback:
            print('repeated...... return')    #????
            # return

        # a new command from gobot_head is received
        g_mqtt.publish(self.message_config.main_mqtt_topic, body)
        self.channel_main.basic_ack(delivery_tag=method.delivery_tag)
        #stop consume a
        print("                       Stop consuming now..", self.message_config.main_mqtt_topic, body)
        self.channel_main.stop_consuming()  # this will break all callbacks
        self.consuming_message_in_queue = False
        self.channel_feedback.queue_declare(queue=self.message_config.feedback_queue)
        self.channel_feedback.basic_consume(queue=self.message_config.feedback_queue, on_message_callback=self.callback_feedback, auto_ack=True )

    def SpinOnce(self) -> None:
        '''
        1. Check mq from MES
        2. Find a AGV
        3. Calculate routing for that AGV
        3. Publish routing message to that AGV
        '''
        mes_task = self.Check_MES_Task()
        if mes_task == None:
            # there is no task from MES
            return
        agv = self.GetFreeAgvBot()
        path_a = self.AppendRoutingNode()
        path_b = self.AppendRoutingNode()
        self.DispatchTask()


    def AppendRoutingNode(self, routing_num:int, node_id:int, subsequence_task:int):
        if routing_num == 1:
            self.first_routing.append(node_id,subsequence_task)
        if routing_num == 2:
            self.second_routing.append(node_id,subsequence_task)

    def Check_MES_Task(self) -> Single_MesTask:
        mes_task = None
        # Check message queue from MES
        return mes_task


    def GetFreeAgvBot(self) -> MapElement_AGV:
        for agvbot in self.mes_resources.all_agvs:
            if agvbot.state == 0:
                return agvbot
        return None


    def DispatchTask(self, load_node_id, unload_node_id):
        '''
        Calulate two routings on the map(determin all branch nodes)
        Find a AGV, and first routing, move from current location to source location,
        second routing: move from source location to target location.
        '''
        agv = self.GetFreeAgvBot()
        if agv is None:
            return
        self.rabbitmq_client.PublishToAgv(agv.id, "G1L123")  # Load at node 123 
        self.rabbitmq_client.PublishToAgv(agv.id, "G1U234")  # Unload at node 234
        # self.first_routing.append(load_node_id)
        # self.second_routing.append(unload_node_id)


class Mes_emulator:
    def __init__(self) -> None:
        helper = RabbitMqClient_Helper()
        self.rabbit_client = helper.MakeClient()
        self.message_queue = 'puma_mes_task'

    def PublishTask(self, new_task: Single_MesTask):
        # payload = json.encoder(new_task)
        JSONData = json.dumps(new_task, indent=4, cls=MyJsonEncoder)
        self.rabbit_client.Publish(self.message_queue, payload=JSONData)

    def PublishBatchTask_ForTest(self):
        for i in range(100):
            task = Single_MesTask()
            task.load_from = i+3
            task.unload_to = i+5
            self.PublishTask(task)



if __name__ == '__main__':
    # emulator = Mes_emulator()
    # emulator.PublishBatchTask_ForTest()
    
    
    mm = MesManager()
    while True:
        mm.SpinOnce()


