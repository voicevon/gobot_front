import json

# from pika import BlockingConnection
from rabbit_mq_basic import RabbitClient, RabbitMQBrokeConfig
from AGV.mes_resources import MapElement_AGV,MapElement_Node, MES_Resources, MyJsonEncoder


from enum import Enum

class BotTaskState(Enum):
    NoPlan = 0
    Planed = 1
    Executing = 2
    Done = 3



class Single_MesTask:
    def __init__(self) -> None:
        self.load_from = MapElement_Node()
        self.unload_to = MapElement_Node()
        self.stated = 0


class MesManager:
    '''
    Receive message from queue topic='puma/mes',
        The message include LoadFromStation, UnloadToStation
    publish message to queue topic ='puma/agv/v123' \n
    The agv will dispatch loading/unloading to box_mover.

    TODO:  Current mes_task is callbacked when message queue got a task. Means running not in main thread.
            We want to check mq, when got free AGV
    '''
    def __init__(self) -> None:
        '''
        First, We initialize our resources: 
        1. Map:  RoadNode, branch-side.
        2. Workstations. Where can do loading, unloading, charging, parking.
        3. Agvs. All Agvs we can schedule.
        Second, Connect to RabbitMQ, In future, We can:
        1. Receive Message from Mes Manager.
        2. Publish Message to AGVs
        '''
        self.mes_resources = MES_Resources()
        self.mes_task = Single_MesTask()

        config = RabbitMQBrokeConfig()
        self.connection = RabbitClient(config).ConnectToRabbitMq()
        self.mq_rx_channel = self.connection.channel()
        self.mes_task_queue_name = 'puma_mes_task'
        self.mq_rx_channel.queue_declare(queue=self.mes_task_queue_name)
        self.mq_rx_channel.basic_consume(queue=self.mes_task_queue_name, on_message_callback=self.mes_task_rx_callback, auto_ack=False )

    def SpinOnce(self) -> None:
        '''
        1. Check mq from MES
        2. Find a AGV
        3. Calculate routing for that AGV
        3. Publish routing message to that AGV
        '''
        if self.mes_task.stated == BotTaskState.NoPlan:
            # Try to get a free agv.
            agv = self.GetFreeAgvBot()
            if agv is None:
                return
            #Calculate path
            path_a = int[]
            path_b = int[]

            # send the path to the free agv.

            # empty mes_task
            self.mes_task.stated = BotTaskState.Planed


        if self.mes_task.stated != BotTaskState.NoPlan:
            # path_a = self.AppendRoutingNode()
            # path_b = self.AppendRoutingNode()
            # self.DispatchTask(path_a, path_b)
            if self.mq_rx_channel._consumer_infos:
                self.mq_rx_channel.connection.process_data_events(time_limit=0.1)  # will blocking 0.1 second

    def mes_task_rx_callback(self, ch, method, properties, body):
        print(' mes_task_rx_callback()  Received ' ,  method.routing_key, body)
        if self.mes_task.stated != BotTaskState.NoPlan:
            # from json to mes_task
            xx = json.loads(body)
            self.mes_task.load_from = xx["load_from"]
            self.mes_task.unload_to = xx['unload_to']
            self.mes_task.stated = BotTaskState.NoPlan
        
            self.mq_rx_channel.basic_ack(delivery_tag=method.delivery_tag)
            # without ack, will rabbit mq cause this callback hannpend again?   should be yes, but how long ?

    def AppendRoutingNode(self, routing_num:int, node_id:int, subsequence_task:int):
        if routing_num == 1:
            self.first_routing.append(node_id,subsequence_task)
        if routing_num == 2:
            self.second_routing.append(node_id,subsequence_task)

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
        config = RabbitMQBrokeConfig()
        config.uid = "von"
        config.password = "von1970"
        self.client=RabbitClient(config)
        self.connection = self.client.ConnectToRabbitMq()
        self.message_queue = 'puma_mes_task'

    def PublishTask(self, new_task:Single_MesTask):
        JSONData = json.dumps(new_task, indent=4, cls=MyJsonEncoder)
        # JSONData = json.dumps(self.all_map_elements, indent=4, cls=MyJsonEncoder)
        print(JSONData)
        self.client.Publish(self.message_queue, payload=JSONData)

    def PublishBatchTask_ForTest(self):
        for i in range(100):
            task = Single_MesTask()
            task.load_from = i+3
            task.unload_to = i+5
            self.PublishTask(task)



if __name__ == '__main__':
    todo = 2
    if todo==1:
        emulator = Mes_emulator()
        emulator.PublishBatchTask_ForTest()
    
    if todo == 2:    
        mm = MesManager()
        while True:
            mm.SpinOnce()


