# from pika import BlockingConnection
from rabbit_mq_basic import RabbitClient, RabbitMQBrokeConfig
from AGV.mes_elements import FileNames, MyJsonEncoder, Single_MesTask, MapElement_Robot, MapElement_Node
from rabbitmq_mqtt_sync import RabbitMQSyncer
from AGV.mes_resources_init_files import MES_ResourcesHelper

import json


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
    def __init__(self) -> None:
        self.mes_task = 'puma_mes_task'
        self.robot_state = 'puma_bot_state'
        self.to_robot_topic_base = 'puma_bot_xnnnn'


    def ForThisRobot(self, robot:MapElement_Robot) -> str:
        queue_name = self.to_robot_topic_base.replace('nnnn', str(robot.id))
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

        # self.mes_task = Single_MesTask()
        # self.mes_task.state = BotTaskState.Planed
        self.mes_task_json = None
        self.mes_task_ack = None
        self.path_to_load = [] 
        self.path_to_unload = []
        self.all_map_nodes=[]
        self.all_robots = []
        self.all_syncers=[]
        self.LoadMapNodes_FromJsonFile(FileNames.MapNodes)

    def Init_MessageQueue_RxTx(self):
        config = RabbitMQBrokeConfig()
        self.mq_client = RabbitClient(config)
        self.mq_connection = self.mq_client.ConnectToRabbitMq()
        # receive mes_task
        self.mq_rx_channel_mes_task = self.mq_connection.channel()
        self.mq_rx_channel_mes_task.basic_qos(prefetch_count=1)
        self.mq_rx_channel_mes_task.queue_declare(queue=MqNames().mes_task)
        self.mq_rx_channel_mes_task.basic_consume(queue=MqNames().mes_task, on_message_callback=self.mes_task_rx_callback, auto_ack=False )
        # To receive robot-states
        self.mq_rx_channel_robot_state = self.mq_connection.channel()
        self.mq_rx_channel_robot_state.basic_qos(prefetch_count=1)
        self.mq_rx_channel_robot_state.queue_declare(queue=MqNames().robot_state)
        self.mq_rx_channel_robot_state.basic_consume(queue=MqNames().robot_state, on_message_callback=self.robot_state_rx_callback, auto_ack=False)

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

    def MakeRobotMqSyncer(self, robot:MapElement_Robot) -> None:
        queue_name = MqNames().ForThisRobot(robot)
        print(queue_name)
        syncer = RabbitMQSyncer(self.mq_connection, queue_name)
        self.all_syncers.append(syncer)

    def SpinOnce(self) -> None:
        '''
        1. Check mq from MES
        2. Find a AGV
        3. Calculate routing for that AGV
        3. Publish routing message to that AGV
        '''
        self.DealwithRobot_LowBattery()

        # if self.mq_rx_channel_mes_task._consumer_infos:
            #TODO:  solve problem: Sometime, after this line, there is no callback. And will be recoverd after paused for 5 to 100 seconds 
        self.mq_rx_channel_mes_task.connection.process_data_events(time_limit=0.01)  # will blocking 0.1 second
        if self.mes_task_ack is  None:
            return
        robot = self.GetRobot_FirstIdle()
        if robot is None:
            return
        print('[Info] MesManager.SpinOnce()    Got task and idle robot...')
        self.mq_rx_channel_mes_task.basic_ack(delivery_tag=self.mes_task_ack)
        self.mes_task_ack = None
        task_json = json.loads(self.mes_task_json)
        mes_task = Single_MesTask()
        mes_task.load_from_node_id = task_json["load_from"]

        #Calculate path to load, and unload.
        print("Trying to calculate path")
        # print(self.mes_task.load_from_node_id)
        self.CalculatePath(mes_task.load_from_node_id, mes_task.unload_to_node_id)
        # send the path to that agv.
        print("Trying to dispatch task to robot")
        self.DispatchTaskTo(robot)
        # empty mes_task

        # TODO: Let robot to charging/sleeping/Wakeingup

            # Try to get a free agv.
            # print("Trying to find idle robot")




    def mes_task_rx_callback(self, ch, method, properties, body):
        # from json to mes_task
        print('[Info] MesManager. mes_task_rx_callback()  ' )
        if self.mes_task_ack is None:
            self.mes_task_json = body
            self.mes_task_ack = method.delivery_tag
        else:
            print('[Info] MesManager. mes_task_rx_callback()    How can this happened? ')
        # self.mes_task.load_from_node_id = jj["load_from"]
        # self.mes_task.unload_to_node_id = jj['unload_to']
        # self.mes_task.state = BotTaskState.NoPlan
        # self.mq_rx_channel_mes_task.basic_ack(delivery_tag=method.delivery_tag)

    def robot_state_rx_callback(self, ch, method, properties, body):
        print('[Info] MesManager.robot_state_rx_callback()      body= ', body)
        xx = json.loads(body)
        robot_id = xx["id"]
        the_robot = self.GetRobot_FromId(robot_id)
        if the_robot is None:
            print("[Info] MesManager.robot_state_rx_callback()   Making new syncer.", robot_id)
            new_robot = MapElement_Robot(robot_id)
            self.all_robots.append(new_robot)
            self.MakeRobotMqSyncer(new_robot)
            the_robot = new_robot
        the_robot.location = xx["nd"]
        the_robot.battery_voltate = xx["bat"]
        the_robot.state = xx["sta"]
        self.mq_rx_channel_robot_state.basic_ack(delivery_tag=method.delivery_tag)

        return

    def CalculatePath(self, node_id_to_load:int, node_id_to_unload:int) -> None:
        # check from map_node
        # self.mes_resources.all_map_nodes[]
        self.path_to_load=[]
        self.path_to_load.append(node_id_to_load)  # might be shorter path

        self.path_to_unload=[]
        self.path_to_unload.append(node_id_to_unload)  

    def GetRobot_FirstIdle(self) -> MapElement_Robot:
        for robot in self.all_robots:
            if robot.state == 0:
                return robot
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
        if robot.state != 0:
            # Not idle
            return 
        robot.state = 1   # Syncing command

        # Got a robot with low battery.  Shold move to charge station
        # Calculate path to charge station, 
        payloads = []
        for node_id in self.path_to_unload:
            payloads.append(str(node_id))
        payloads.append('charge')

        queue_name = MqNames().ForThisRobot( robot)
        self.mq_client.PublishBatch(queue_name,  payloads)  # Load at node 123


    def DispatchTaskTo(self, robot:MapElement_Robot):
        '''
        Calulate two routings on the map(determin all branch nodes)
        Find a AGV, and first routing, move from current location to source location,
        second routing: move from source location to target location.
        '''
        payloads = []
        for node_id in self.path_to_load: 
            payloads.append(str(node_id))
        payloads.append('load')

        for node_id in self.path_to_unload:
            payloads.append(str(node_id))
        payloads.append('unload')

        queue_name = MqNames().ForThisRobot(robot)
        self.mq_client.PublishBatch(queue_name,  payloads)  # Load at node 123 


if __name__ == '__main__':
        from von.mqtt_agent import g_mqtt,g_mqtt_broker_config
        g_mqtt.connect_to_broker(g_mqtt_broker_config)

        mm = MesManager()
        while True:
            mm.SpinOnce()


