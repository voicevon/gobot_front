import json
# from pika import BlockingConnection
from rabbit_mq_basic import RabbitClient, RabbitMQBrokeConfig
from AGV.mes_elements import FileNames, MyJsonEncoder, Single_MesTask, MapElement_Robot, MapElement_Node
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
    robot_state = 'puma/bot/state'
    to_agv_topic_base = 'puma_agv_xxx'


    def ForThisRobot(self, agv:MapElement_Robot) -> str:
        print(agv.id)
        queue_name = self.to_agv_topic_base.replace('xxx', str(agv.id))
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
        self.Init_MessageQueue_RxTx()
        self.Init_Elements()

        self.mes_task = Single_MesTask()
        self.path_to_load = [] 
        self.path_to_unload = []

    def Init_MessageQueue_RxTx(self):
        self.all_syncers=[]
        config = RabbitMQBrokeConfig()
        self.mq_client = RabbitClient(config)
        self.mq_connection = self.mq_client.ConnectToRabbitMq()
        # receive mes_task
        self.mq_rx_channel_mes_task = self.mq_connection.channel()
        self.mq_rx_channel_mes_task.queue_declare(queue=MqNames.mes_task)
        self.mq_rx_channel_mes_task.basic_consume(queue=MqNames.mes_task, on_message_callback=self.mes_task_rx_callback, auto_ack=False )
        # To receive robot-states
        self.mq_rx_channel_robot_state = self.mq_connection.channel()
        self.mq_rx_channel_robot_state.queue_declare(queue=MqNames.robot_state)
        self.mq_rx_channel_robot_state.basic_consume(queue=MqNames.robot_state, on_message_callback=self.robot_state_rx_callback, auto_ack=False)

    def Init_Elements(self):
        self.all_map_nodes=[]
        self.LoadMapNodes_FromJsonFile(FileNames.MapNodes)

        self.all_robots = []
        self.__LoadAgvs_FromJsonFile(FileNames.MapAgvs)
        # aa = MapElement_Robot(22)
        # print(aa.id)
        for aa in self.all_robots:
            print(aa.id)
            self.MakeRobotMqSyncer(aa)
        # mes_resources = MES_ResourcesHelper()
        # self.all_map_nodes = mes_resources.all_map_nodes

    def __LoadAgvs_FromJsonFile(self, filename:str):
        # Do every steps again for agv
        file = open(filename,"r")
        data = file.read()
        file.close()
        str_json = json.loads(data)
        # all_robots=[MapElement_Robot]
        for bot_json in str_json:
            new_robot = MapElement_Robot(0)
            new_robot.id = bot_json["id"]
            # TODO: more members
            self.all_robots.append(new_robot)
            print (new_robot.id)
        # return all_robots
    
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

    def MakeRobotMqSyncer(self, agv:MapElement_Robot) -> None:
        queue_name = MqNames.ForThisRobot(agv)
        sync = RabbitMQSync(self.mq_connection, queue_name)
        self.all_syncers.append(sync)

    def SpinOnce(self) -> None:
        '''
        1. Check mq from MES
        2. Find a AGV
        3. Calculate routing for that AGV
        3. Publish routing message to that AGV
        '''

        # TODO: Check robot is online/offline

        # Check battery voltage
        self.DealwithRobot_LowBattery()
        # TODO: Let agv to charging/sleeping/Wakeingup
        for syncer in self.all_syncers:
            syncer.SpinOnce()

        if self.mes_task.state == BotTaskState.NoPlan:
            # Try to get a free agv.
            agv = self.GetFreeAgvRobot()
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


    def robot_state_rx_callback(self, ch, method, properties, body):
        xx = json.loads(body)
        the_agv = self.GetRobot_FromId(xx["id"])
        the_agv.id = xx["id"]
        the_agv.location = xx["location"]
        the_agv.battery_voltate = xx["bat"]


    def CalculatePath(self, node_id_to_load:MapElement_Node, node_id_to_unload:MapElement_Node) -> None:
        # check from map_node
        # self.mes_resources.all_map_nodes[]
        self.path_to_load=[MapElement_Node]
        self.path_to_load.append(node_id_to_load)  # might be shorter path

        self.path_to_unload=[MapElement_Node]
        self.path_to_unload.append(node_id_to_unload)  

    def GetFreeAgvRobot(self) -> MapElement_Robot:
        for agv in self.all_robots:
            if agv.state == 0:
                return agv
        return None

    def GetRobot_FromId(self, robot_id:int) -> MapElement_Robot:
        for robot in self.all_robots:
            if robot.id == robot_id:
                return robot
        return None

    def GetRobot_LowBattery(self) -> MapElement_Robot:
        for robot in self.all_robots:
            if robot.battery_voltate < 9.50:
                return robot
        return None

    def DealwithRobot_LowBattery(self):
        robot = self.GetRobot_LowBattery()
        if robot is None:
            return
        # Got a robot with low battery.  Shold move to charge station
        # Calculate path to charge station, 
        payload = [str]
        for node in self.path_to_unload:
            payload.append(node.Node_id)
        payload.append('charge')

        queue_name = MqNames.ForThisRobot(robot)
        self.mq_client.PublishBatch(queue_name,  payload)  # Load at node 123 


    def DispatchTask(self, robot:MapElement_Robot):
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

        queue_name = MqNames.ForThisRobot(robot)
        self.mq_client.PublishBatch(queue_name,  payload)  # Load at node 123 


if __name__ == '__main__':
        mm = MesManager()
        while True:
            mm.SpinOnce()


