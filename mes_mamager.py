from robot_tasks_scheduler.bot_tasks_queue import TaskQueue
from rabbitmq_all_in_one import RabbitClient, RabbitMqClient_Helper


from AGV.mes_resources import MapElement_AGV,MapElement_Node, MES_Resources


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

    def SpinOnce(self) -> None:
        '''
        1. Check mq from MES
        2. Find a AGV
        3. Calculate routing for that AGV
        3. Publish routing message to that AGV
        '''
        mes_task = self.Check_MES_MessageQueue()
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
    def Check_MES_MessageQueue(self) -> MesTask:
        mt = MesTask()
        return mt


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

        
        







if __name__ == '__main__':
    aa = AgvManager()
