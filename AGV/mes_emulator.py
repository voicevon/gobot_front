import sys
sys.path.append('C:\\gitlab\\gobot_front')  # For runing in VsCode on Windows-10 


from rabbit_mq_basic import RabbitMQBrokeConfig, RabbitClient
from AGV.mes_elements import MyJsonEncoder, Single_MesTask
import json


# Instruction on windows-10
#   cd C:\gitlab\gobot_front\AGV>
#   python3 mes_emulator.py


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
        # print(JSONData)
        self.client.Publish(self.message_queue, payload=JSONData)

    def PublishBatchTask_ForTest(self):
        for i in range(12345):
            task = Single_MesTask()
            task.load_from = i+3
            task.unload_to = i+5
            self.PublishTask(task)



if __name__ == '__main__':
    todo = 1
    if todo==1:
        emulator = Mes_emulator()
        emulator.PublishBatchTask_ForTest()