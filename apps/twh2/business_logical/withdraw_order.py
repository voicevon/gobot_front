from logger import Logger
from database.db_withdraw_order import DB_WithdrawOrder
from wcs_robots.twh_robot_packer import TwhRobot_Packer
from wcs_robots.twh_robot_shipper import TwhRobot_Shipper
from database.bolt_nut import twh_factory

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
    def __init__(self, order_id:int, packer:TwhRobot_Packer, shipper:TwhRobot_Shipper) -> None:
        self.__all_teeth = []
        self.Order_id = order_id
        self.PackerCell_id = -1
        self.__state = 'idle'
        self.__packer = packer
        self.__shipper = shipper

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
        * 'wms_shipping'
        * 'wcs_shipping'
        * 'shipped'
        '''
        self.__state = new_state
        if write_to_db:
                doc_ids = self.__get_all_teeth_doc_ids()
                DB_WithdrawOrder.update_order_state(new_state, doc_ids)

    #TODO: a better name:  Start_PickingPlacing()   StartTransfer()   
    # def Start_PickingPlacing_a_tooth(self, idle_packer_cell_id:int):
    def Start_PickingPlacing_a_tooth(self) -> bool:
        if self.__state == 'idle':
            # this is the first tooth of the order. 
            idle_packer_cell_id =  self.__packer.Find_Idle_packer_cell()
            if idle_packer_cell_id == -1:
                return False
            self.PackerCell_id = idle_packer_cell_id
            self.__packer.StartFeeding_LockPackerCell(idle_packer_cell_id)
            self.SetStateTo('feeding', write_to_db=True)
        return True    
            
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
            if self.IsFullFilled():
                doc_ids = self.__get_all_teeth_doc_ids()
                DB_WithdrawOrder.update_order_state('fullfilled', doc_ids)
                return False
        if self.__state == 'fullfilled':
            return False
        
        if self.__state == 'wms_shipping':
            if self.__shipper.IsShipping():
                return False
            self.__packer.StartShipping(self.PackerCell_id)
            self.__shipper.StartShipping() 
            # multiple orders is in the state of 'wms_shipping'
            doc_ids = self.__get_all_teeth_doc_ids()
            DB_WithdrawOrder.update_order_state('wcs_shipping', doc_ids)
            return False

        if self.__state == 'wcs_shipping':
            if self.__shipper.Get_Shipout_button_value()=='ON':
                self.__shipper.EndShipping()

                DB_WithdrawOrder.delete_by_order_id(self.Order_id)
                self.__packer.Release_packer_cell(self.PackerCell_id)
                return True
        return False


class WithdrawOrderManager():
    def __init__(self, twh_id:str, packer:TwhRobot_Packer, shipper:TwhRobot_Shipper) -> None:
        self.__all_order_tasks = []
        self.__twh_id = twh_id
        self.__packer = packer
        self.__shipper = shipper

    def AddOrderTask(self, new_order_task: WithdrawOrder):
        self.__all_order_tasks.append(new_order_task)

    def FindOrderTask(self, order_id:str) -> WithdrawOrder:
        for order_task in self.__all_order_tasks:
            if order_task.Order_id == order_id:
                return order_task
        return None # type: ignore
    
    def GetTasksCount(self) -> int:
        return len(self.__all_order_tasks)
    
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
        
    def __renew_orders_from_database(self):
        '''
        1. renew all orders from database
        2. renew teeth state inside order (the state is from database)
        3. turorial note: https://tinydb.readthedocs.io/en/latest/usage.html
        The TinyDB query cache doesn't check if the underlying storage that the database uses has been modified by an external process. 
        In this case the query cache may return outdated results. 
        To clear the cache and read data from the storage again you can use db.clear_cache().
                
        '''
        # Logger.Debug('Twh_WarehouseControlSystem:: renew_order_state_from_database()')
        printed_logger_title = False
        DB_WithdrawOrder.table_withdraw_order.clear_cache()
        db_order_teeth =  DB_WithdrawOrder.table_withdraw_order.all()
        for db_tooth in db_order_teeth:
            the_order = self.FindOrderTask(db_tooth['order_id'])
            if db_tooth['twh_id'] == self.__twh_id:   # TODO:  move into db_order_teeth  searching.
                if the_order is None:
                    new_order = WithdrawOrder(db_tooth['order_id'], self.__packer, self.__shipper)
                    self.AddOrderTask(new_order)
                    the_order = new_order
                    if not printed_logger_title:
                        Logger.Debug('WithdrawOrderManager::__renew_orders_from_database() First')
                        Logger.Print('Factory_name', twh_factory[self.__twh_id])
                        printed_logger_title = True
                    Logger.Print('WithdrawOrderManager::__renew_orders_from_database()   new_order_task is added to manager. Order_id', new_order.Order_id)
                the_order.SetStateTo(db_tooth['order_state'], write_to_db=False)

                order_tooth = the_order.FindTooth_from_doc_id(db_tooth.doc_id)
                if order_tooth is None:
                    new_tooth = OrderTooth(db_tooth.doc_id)
                    new_tooth.DentalLocation = db_tooth['location']
                    new_tooth.row = db_tooth['row']
                    new_tooth.col = db_tooth['col']
                    new_tooth.layer = db_tooth['layer']
                    the_order.AddTooth(new_tooth)
                    order_tooth = new_tooth
                    if not printed_logger_title:
                        Logger.Debug('WithdrawOrderManager::__renew_orders_from_database()  Second')
                    Logger.Print('new_tooth is added to order_task. DentalLocation', new_tooth.DentalLocation)
                order_tooth.TransferToLocated(db_tooth['located'], write_to_db=False)

            # if order_task.GetState() == 'shipped':
            #     DB_WithdrawOrder.delete_by_order_id(order_task.Order_id)
            #     self.__all_order_tasks.remove(order_task)

    def SpinOnce(self):
        '''
        return:
            -1   no released packer_cell
            0:11 packer_cell_id,  which has benn shipped out. should be released.
        '''
        self.__renew_orders_from_database()
        
        for order in self.__all_order_tasks:
            is_shipped =  order.SpinOnce()
            if is_shipped:
                Logger.Info( twh_factory[self.__twh_id]['name'] +  ' -- WithdrawOrderManager:: SpinOnce().  Order is shipped')
                self.__all_order_tasks.remove(order)
                return


