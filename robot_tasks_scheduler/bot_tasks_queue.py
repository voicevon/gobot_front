
from enum import Enum

class BotTaskState(Enum):
    NoPlan = 0
    Planed = 1
    Executing =2
    Done = 3

# TASK_STATE = {"NoPlan":1, "Planed":2, "MovingToSouce":3}

class BotTask:
    def __init__(self, task_id:int) -> None:
        self.task_id = task_id
        self.SourceSite_id = 0
        self.TargetSite_id = 0
        self.State = BotTaskState.NoPlan
        self.Bot_id = 0
    

    
class TaskQueue():
    tasks = []
    
    @classmethod
    def FetchSingleTask(cls) -> BotTask:
        for task in cls.tasks:
            if task.State == BotTaskState:
                return task
        return None

    def AppendTask(cls, task_id:int, source_station: int, target_station:int)->bool:
        task = BotTask(task_id)
        task.SourceSite_id = source_station
        task.TargetSite_id = target_station
        cls.tasks.append(task)

    @classmethod
    def SpinOnce(cls):
        '''
        Try to tell targetbot its task
        '''


    @classmethod
    def __str__(cls):
        return 'tasks len() =' + str(cls.tasks.__len__())

    __repr__ = __str__
    show = __str__