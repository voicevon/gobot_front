from logger import Logger
from database.db_withdraw_order import DB_WithdrawOrder

class OrderTooth():
    def __init__(self, db_doc_id:int) -> None:
        self.doc_id = db_doc_id
        self.DentalLocation = 'ur1'
        self.row :int
        self.col:int
        self.layer:int
        # self.packer_cell_id:int
        self.__located = 'porter'

    def TransferToLocated(self, new_located:str, write_to_db:bool) -> None:
        self.__located = new_located
        if write_to_db:
            DB_WithdrawOrder.update_tooth_located(self.doc_id, new_located)

    def GetLocated(self) -> str:
        '''
        'porter', 'worker_hand','packer_cell'
        '''
        return self.__located
    
    def PrintOut(self, title:str):
        Logger.Info(title)
        Logger.Print('doc_id', self.doc_id)
        Logger.Print('DentalLocation', self.DentalLocation)
        Logger.Print('__located', self.__located)
        Logger.Print('row', self.row)
        Logger.Print('col', self.col)
        Logger.Print('layer', self.layer)


class WithdrawOrder():
    def __init__(self, order_id:str) -> None:
        self.__all_teeth = []
        self.Order_id = order_id
        self.PackerCell_id = -1
        self.__state = 'idle'

    def AddTooth(self, new_tooth:OrderTooth) -> None:
        self.__all_teeth.append(new_tooth)
    
    def FindTooth_from_doc_id(self, doc_id:int) -> OrderTooth:
        for t in self.__all_teeth:
            if t.doc_id == doc_id:
                return t
        return None # type: ignore

    def FindTooth_is_in_porter(self, porter_id:int) -> OrderTooth:
        '''
        * porter_id is equal to tooth.row.
        * constraint:  tooth must be located in porter
        '''
        # Logger.Debug('WithdrawOrder:: FindTooth_is_in_porter() ')
        for tooth in self.__all_teeth:
            # tooth.PrintOut('WithdrawOrder:: FindTooth_is_in_porter(), __all_teeth.this tooth')
            # Logger.Print('located', tooth.GetLocated())
            if tooth.GetLocated() == 'porter':
                if tooth.row == porter_id:
                    return tooth
        return None # type: ignore

    def HasTooth(self, tooth:OrderTooth) -> bool:
        for t in self.__all_teeth:
            if tooth == t:
                return True
        return False
    
    def __get_all_teeth_doc_ids(self):
        doc_ids = []
        for tooth in self.__all_teeth:
            doc_ids.append(tooth.doc_id)
        return doc_ids
    
    def SetStateTo(self, new_state:str, write_to_db:bool):
        '''
        * 'idle', 
        * 'feeding', 
        * 'fullfilled', 
        * 'shipping'
        * 'shipped'
        '''
        self.__state = new_state
        if write_to_db:
                doc_ids = self.__get_all_teeth_doc_ids()
                DB_WithdrawOrder.update_order_state(new_state, doc_ids)

    def GetState(self) -> str:
        return self.__state

    def IsFullFilled(self) -> bool:
        for t in self.__all_teeth:
            if t.GetLocated() != 'packer':
                return False
            
        return True

    def SpinOnce(self) -> bool:
        '''
        return:
            * True: I am complete shipped out, and has been deleted from database.
            * False: I am not shipped.
        '''
        if self.__state == 'idle':
            return False
        if self.__state == 'feeding':
            if self.IsFullFilled:
                doc_ids = self.__get_all_teeth_doc_ids()
                DB_WithdrawOrder.update_order_state('fullfilled', doc_ids)
                return False
        if self.__state == 'fullfilled':
            return False
        if self.__state == 'shipping':
            return False
        if self.__state == 'shipped':
            DB_WithdrawOrder.delete_by_order_id(self.Order_id)
            return True
        return False


class WithdrawOrderManager():
    def __init__(self) -> None:
        self.__all_order_tasks = []

    def AddOrderTask(self, new_order_task: WithdrawOrder):
        self.__all_order_tasks.append(new_order_task)

    def FindOrderTask(self, order_id:str) -> WithdrawOrder:
        for order_task in self.__all_order_tasks:
            if order_task.Order_id == order_id:
                return order_task
        return None # type: ignore
    
    def GetTasksCount(self) -> int:
        return len(self.__all_order_tasks)
    
    def __FindTooth_is_in_Order(self, tooth:OrderTooth) -> WithdrawOrder:
        for order in self.__all_order_tasks:
            if order.HasTooth(tooth):
                return order
        return None # type: ignore

    # def Transered_into_Packer(self, tooth:OrderTooth):
    #     order  = self.__FindTooth_is_in_Order(tooth)
    #     if order is None:
    #         Logger.Error('OrderTaskManager:: Transered_into_Packer()  ')
    #         tooth.PrintOut('No order has this tooth')
    #         return
        
    #     # No point to do above ??
    #     tooth.TransferToLocated('packer',write_to_db=True)
        
        # check whether all teeth are in packer.
        # Why should not be here?
        # In case of the applications is crashed here. 

    def FindTooth_is_in_porter_from_all_order(self, porter_id:int) -> tuple[OrderTooth, WithdrawOrder]:
        '''
        * porter_id is equal to tooth.row.
        * constraint:  tooth must be located in porter
        '''
        # Logger.Debug('OrderTaskManager:: FindTooth_is_in_porter() ')
        for order in self.__all_order_tasks:
            tooth = order.FindTooth_is_in_porter(porter_id)
            if tooth is not None:
                # Logger.Print('found tooth in the loop-porter,  tooth.col', tooth.col)
                return tooth, order
        return None,None # type: ignore
        
    def renew_orders_from_database(self):
        '''
        1. renew order state
        2. renew teeth state inside order
        3. from https://tinydb.readthedocs.io/en/latest/usage.html
        The TinyDB query cache doesn’t check if the underlying storage that the database uses has been modified by an external process. 
        In this case the query cache may return outdated results. 
        To clear the cache and read data from the storage again you can use db.clear_cache().
                
        '''
        # Logger.Debug('Twh_WarehouseControlSystem:: renew_order_state_from_database()')
        DB_WithdrawOrder.table_withdraw_order.clear_cache()
        db_order_teeth =  DB_WithdrawOrder.table_withdraw_order.all()
        for db_tooth in db_order_teeth:
            order_task = self.FindOrderTask(db_tooth['order_id'])
            if order_task is None:
                new_order_task = WithdrawOrder(db_tooth['order_id'])
                self.AddOrderTask(new_order_task)
                order_task = new_order_task
                Logger.Print('new_order_task is added to manager. Order_id', new_order_task.Order_id)
            order_task.SetStateTo(db_tooth['order_state'], write_to_db=False)

            order_task_tooth = order_task.FindTooth_from_doc_id(db_tooth.doc_id)
            if order_task_tooth is None:
                new_tooth = OrderTooth(db_tooth.doc_id)
                new_tooth.DentalLocation = db_tooth['location']
                new_tooth.row = db_tooth['row']
                new_tooth.col = db_tooth['col']
                new_tooth.layer = db_tooth['layer']
                order_task.AddTooth(new_tooth)
                order_task_tooth = new_tooth
                Logger.Print('new_tooth is added to order_task. DentalLocation', new_tooth.DentalLocation)
            order_task_tooth.TransferToLocated(db_tooth['located'], write_to_db=False)

            # if order_task.GetState() == 'shipped':
            #     DB_WithdrawOrder.delete_by_order_id(order_task.Order_id)
            #     self.__all_order_tasks.remove(order_task)

    def find_shipping_order(self) -> WithdrawOrder:
        for order in self.__all_order_tasks:
            if order.GetState() == 'shipping':
                return order
        return None # type: ignore
        
    def SpinOnce(self) -> int:
        '''
        return:
            -1   no released packer_cell
            0:11 packer_cell_id,  which has benn shipped out. should be released.
        '''
        self.renew_orders_from_database()
        
        for order in self.__all_order_tasks:
            is_shipped  = order.SpinOnce()
            if is_shipped:
                packer_cell_id = order.PackerCell_id
                Logger.Info('OrderTaskManager:: SpinOnce()')
                Logger.Print('This packer cell is released.  packer_cell_id', packer_cell_id)
                self.__all_order_tasks.remove(order)
                return packer_cell_id
        return -1


