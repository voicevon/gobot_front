from logger import Logger
from database.db_withdraw import DB_OrderTask

class OrderTaskTooth():
    def __init__(self, db_doc_id:int) -> None:
        self.doc_id = db_doc_id
        self.DentalLocation = 'ur1'
        self.row = 1
        self.col = 1
        self.layer = 1
        self.__located = 'porter'

    def TransferTo(self, new_located:str) -> None:
        self.__located = new_located

    def GetLocated(self) -> str:
        '''
        'porter', 'worker_hand','packer_cell'
        '''
        return self.__located

class OrderTask():
    def __init__(self, order_id:str) -> None:
        self.__all_teeth = []
        self.Order_id = order_id
        self.__state = 'idle'

    def AddTooth(self, new_tooth:OrderTaskTooth) -> None:
        self.__all_teeth.append(new_tooth)
    
    def FindTooth_from_dental_location(self, dental_location:str) -> OrderTaskTooth:
        for tooth in self.__all_teeth:
            if tooth.DentalLocation == dental_location:
                return tooth
        # Logger.Error('OrderTask:: FindTooth_from_dental_location() ')
        # Logger.Print('dental_location', dental_location)
        return None # type: ignore
    
    def FindTooth_is_in_porter(self, porter_id:int) -> OrderTaskTooth:
        '''
        * porter_id is equal to tooth.row.
        * constraint:  tooth must be located in porter
        '''
        for tooth in self.__all_teeth:
            if tooth.row == porter_id:
                if tooth.GetLocated() == 'porter':
                    return tooth
        return None # type: ignore

    def SetStateTo(self, new_state:str):
        self.__state = new_state

    def GetState(self) -> str:
        return self.__state

class OrderTaskManager():
    def __init__(self) -> None:
        self.__all_order_tasks = []


    def AddOrderTask(self, new_order_task: OrderTask):
        self.__all_order_tasks.append(new_order_task)

    def FindOrderTask(self, order_id:str) -> OrderTask:
        for order_task in self.__all_order_tasks:
            if order_task.Order_id == order_id:
                return order_task
        return None # type: ignore
    
    def GetTasksCount(self) -> int:
        return len(self.__all_order_tasks)
    
    def FindTooth_is_in_porter(self, porter_id:int) -> OrderTaskTooth:
        '''
        * porter_id is equal to tooth.row.
        * constraint:  tooth must be located in porter
        '''
        for order in self.__all_order_tasks:
            tooth = order.FindTooth_is_in_porter(porter_id)
            if tooth is not None:
                return tooth
        return None # type: ignore
        
    def renew_orders_from_database(self):
        '''
        1. renew order state
        2. renew teeth state inside order
        '''
        # Logger.Debug('Twh_WarehouseControlSystem:: renew_order_state_from_database()')
        db_order_teeth =  DB_OrderTask.table_order_task.all()
        for db_tooth in db_order_teeth:
            # Logger.Print('db_tooth',db_tooth)

            order_task = self.FindOrderTask(db_tooth['order_id'])
            if order_task is None:
                new_order_task = OrderTask(db_tooth['order_id'])
                self.AddOrderTask(new_order_task)
                order_task = new_order_task
                Logger.Print('new_order_task is added to manager. Order_id', new_order_task.Order_id)

            order_task.SetStateTo(db_tooth['order_state'])
            order_task_tooth = order_task.FindTooth_from_dental_location(db_tooth['location'])
            if order_task_tooth is None:
                # new_tooth = order_task_tooth(db_tooth['doc_id'])
                new_tooth = OrderTaskTooth(1234)
                new_tooth.DentalLocation = db_tooth['location']
                order_task.AddTooth(new_tooth)
                order_task_tooth = new_tooth
                Logger.Print('new_tooth is added to order_task. DentalLocation', new_tooth.DentalLocation)

            order_task_tooth.TransferTo(db_tooth['located'])