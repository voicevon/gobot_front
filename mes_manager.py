import json
# from pika import BlockingConnection
from rabbit_mq_basic import RabbitClient, RabbitMQBrokeConfig
from AGV.mes_elements import FileNames, MyJsonEncoder, Single_MesTask, MapElement_AGV, MapElement_Node
from rabbitmq_mqtt_sync import RabbitMQSync
from AGV.mes_resources_init_files import MES_ResourcesHelper


from enum import Enum

class BotTaskState(Enum):
    NoPlan = 0
    Planed = 1
    Executing = 2
    Done = 3

# class PathSegment:
#     def __init__(self, map_node:MapElement_Node) -> None:
#         self.map_node 

class MqNames:
    mes_task = 'puma_mes_task'
    agv_mqtt_topic_base = 'puma_agv_xxx'

    def ForThisAgv(self, agv:MapElement_AGV) -> str:
        print(agv.id)
        queue_name = self.agv_mqtt_topic_base.replace('xxx', str(agv.id))
        return queue_name




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
        self.Init_MessageQueue()
        self.Init_Elements()

        self.mes_task = Single_MesTask()
        self.path_to_load = [] 
        self.path_to_unload = []

    def Init_MessageQueue(self):
        self.all_syncers=[]
        config = RabbitMQBrokeConfig()
        self.mq_client = RabbitClient(config)
        self.mq_connection = self.mq_client.ConnectToRabbitMq()
        self.mq_rx_channel = self.mq_connection.channel()
        self.mq_rx_channel.queue_declare(queue=MqNames.mes_task)
        self.mq_rx_channel.basic_consume(queue=MqNames.mes_task, on_message_callback=self.mes_task_rx_callback, auto_ack=False )
    
    def Init_Elements(self):
        self.all_map_nodes=[]
        self.LoadMapNodes_FromJsonFile(FileNames.MapNodes)

        self.all_agvs = []
        self.__LoadAgvs_FromJsonFile(FileNames.MapAgvs)
        # aa = MapElement_AGV(22)
        # print(aa.id)
        for aa in self.all_agvs:
            print(aa.id)
            self.MakeAgvMqSyncer(aa)
        # mes_resources = MES_ResourcesHelper()
        # self.all_map_nodes = mes_resources.all_map_nodes

    def __LoadAgvs_FromJsonFile(self, filename:str):
        # Do every steps again for agv
        file = open(filename,"r")
        data = file.read()
        file.close()
        str_json = json.loads(data)
        # all_agvs=[MapElement_AGV]
        for agv_json in str_json:
            new_agv = MapElement_AGV(0)
            new_agv.id = agv_json["id"]
            # TODO: more members
            self.all_agvs.append(new_agv)
            print (new_agv.id)
        # return all_agvs
    
    def LoadMapNodes_FromJsonFile(self, filename:str) -> None:
        '''
        TODO:  Load AGV when that agv is online.
        '''
        # read file to string
        file = open(filename, "r") 
        data = file.read() 
        file.close()
        # Get json object from string
        str_json = json.loads(data)
        # Copy from json object to my objects
        for node in str_json:
            new_node = MapElement_Node()
            new_node.Node_id = node["RfCard_id"]
            # TODO: more members
            self.all_map_nodes.append(new_node)
            print (new_node.Node_id)

    def MakeAgvMqSyncer(self, agv:MapElement_AGV) -> None:
        queue_name = MqNames().ForThisAgv(agv)
        sync = RabbitMQSync(self.mq_connection, queue_name)
        self.all_syncers.append(sync)

    def SpinOnce(self) -> None:
        '''
        1. Check mq from MES
        2. Find a AGV
        3. Calculate routing for that AGV
        3. Publish routing message to that AGV
        '''

        # TODO: Check agv online/offline
        # TODO: Let agv to charging/sleeping/Wakeingup
        for syncer in self.all_syncers:
            syncer.SpinOnce()

        if self.mes_task.state == BotTaskState.NoPlan:
            # Try to get a free agv.
            agv = self.GetFreeAgvBot()
            if agv is None:
                return
            #Calculate path to load, and unload.
            self.CalculatePath(self.mes_task.load_from.Node_id, self.mes_task.unload_to.Node_id)
            # send the path to that agv.
            self.DispatchTask(agv)
            # empty mes_task
            self.mes_task.state = BotTaskState.Planed

        if self.mes_task.state != BotTaskState.NoPlan:
            if self.mq_rx_channel._consumer_infos:
                self.mq_rx_channel.connection.process_data_events(time_limit=0.1)  # will blocking 0.1 second

    def mes_task_rx_callback(self, ch, method, properties, body):
        print(' mes_task_rx_callback()  Received ' ,  method.routing_key, body)
        if self.mes_task.state != BotTaskState.NoPlan:
            # from json to mes_task
            xx = json.loads(body)
            self.mes_task.load_from = xx["load_from"]
            self.mes_task.unload_to = xx['unload_to']
            self.mes_task.state = BotTaskState.NoPlan
        
            self.mq_rx_channel.basic_ack(delivery_tag=method.delivery_tag)
            # without ack, will rabbit mq cause this callback hannpend again?   should be yes, but how long ?

    def CalculatePath(self, node_id_to_load:MapElement_Node, node_id_to_unload:MapElement_Node) -> None:
        # check from map_node
        # self.mes_resources.all_map_nodes[]
        self.path_to_load=[MapElement_Node]
        self.path_to_load.append(node_id_to_load)  # might be shorter path

        self.path_to_unload=[MapElement_Node]
        self.path_to_unload.append(node_id_to_unload)  

    def GetFreeAgvBot(self) -> MapElement_AGV:
        for agv in self.all_agvs:
            if agv.state == 0:
                return agv
        return None

    def DispatchTask(self, agv:MapElement_AGV):
        '''
        Calulate two routings on the map(determin all branch nodes)
        Find a AGV, and first routing, move from current location to source location,
        second routing: move from source location to target location.
        '''
        payload = [str]
        for node in self.path_to_load: 
            payload.append(node.Node_id)
        payload.append('load')

        for node in self.path_to_unload:
            payload.append(node.Node_id)
        payload.append('unload')

        queue_name = MqNames.ForThisAgv(agv)
        self.mq_client.PublishBatch(queue_name,  payload)  # Load at node 123 



if __name__ == '__main__':
        mm = MesManager()
        while True:
            mm.SpinOnce()


