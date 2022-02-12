
import queue
from bot_tasks_queue import TaskQueue

class BotTaskCommu():

    queue: TaskQueue

    @classmethod
    def SpinOnce(cls):
        '''
        1. Read the State of target bot.
        2. Pick a task, Try to Send the task to target bot.
        '''
        cls.queue.FetchSingleTask()
