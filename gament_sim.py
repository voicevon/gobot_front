from robot_tasks_scheduler.bot_tasks_queue import TaskQueue
from rabbitmq_all_in_one import RabbitClient, RabbitMqClient_Helper
class AgvRoutingNode:
    '''
    After a detail node, 
    1. the agv will follow branch track, not main track.
    2. the box mover will do one of the following task: Nothing, loading, unloading.
    '''
    def __init__(self) -> None:
        self.GoBranch_AfterThisNodeId = 1
        self.TaskAfterThisNode = "Loading"

class AgvRouting:
    '''
    A full routing of a top level task.
    After a detail node, 
    1. the agv will follow branch track, not main track.
    2. the box mover will do one of the following task: Nothing, loading, unloading.

    TODO:  a better class name
    '''
    def __init__(self) -> None:
        self.first_routing = []
        self.second_routing = []
    
    def AppendRoutingNode(self, routing_num:int, node_id:int, subsequence_task:int):
        if routing_num == 1:
            self.first_routing.append(node_id,subsequence_task)
        if routing_num == 2:
            self.second_routing.append(node_id,subsequence_task)


class GarmentMap:
    def __init__(self) -> None:
        # TODO: Load map from config file.s
        self.all_nodes = []

    def AppendNode(self, node_id, next_node_id, next_branch_node_id):
        self.all_nodes.append(node_id, next_node_id, next_branch_node_id)
    
    def CreateRouting_ToNode(self, begin_node_id, end_node_id):
        routing = []
        # Search map, find the source node
        for node in self.all_nodes:
            if begin_node_id==node.id:
                # Got the node, now use recursion.
                pass
        return routing


class AgvBot:
    # after hardware reseting,  AGV will move forward, 
    # after detecting the first location mark, will go to branch and goto idle.
    def __init__(self, id:int) -> None:
        self.id = id
        self.location = -1   # means unknown.
        self.State = -1   # Charging, moving to load, loading, moving to unload, unloading, idle.
        self.battery_voltate = 12.0

    def on_mqtt_message(self, location_id, battery_voltage, state):
        self.location =  location_id
        self.battery_voltate = battery_voltage
        self.State =  state


class AgvManager:
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
        self.agvs=[]
        for i in range(5):
            new_agv = AgvBot()
            self.agvs.append(new_agv)
        self.first_routing = []
        self.second_routing = []
        helper = RabbitMqClient_Helper()
        self.rabbitmq_client = helper.MakeClient()
        self.LoadMap()

    def LoadMap(self)->None:
        '''
        Map include many RodeNode.
        
        '''
        self.RoadNode

    def GetFreeAgvBot(self) -> AgvBot:
        for agvbot in self.agvs:
            if agvbot.State == 0:
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

        
        







if __name__ == '__main__':
    aa = AgvManager()
