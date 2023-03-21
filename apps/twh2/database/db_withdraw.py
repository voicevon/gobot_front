from tinydb import TinyDB, Query, where
from bolt_nut import get_row_from_tooth_location
from database.db_stock import db_Stock

from logger import Logger


class DB_OrderTask():
    '''
    About table_order
        1. Creator:   user request
            state = 'idle'
            linked_packer_cell = -1
        2. Dispacher actions:
            1. set linked_packer_cell = (0..11)
            2. set State= 'feeding', or 'fullfilled'
        3. Deleter:  wcw.Packer, or wms ??

    About order.tooth.located
        'warehouse'    For future wcs application
        'porter'       default value.
        'worker_hand'  when porter is ready, and mono green led is turned on.
        'packer_cell'  When green button is pressed.

    About packer_cell.State
        'idle',        default value when created.
        'feeding',     after linked to a packer-cell
        'fullfilled',  when all teeth moved to packer-cell. saying: the last tooth is confirmed by green button is pressed.
        'packed'       when bule buton is pressed. saying:  set by wcs, delete by wms ??
                       when deleting, will insert into 'table_withdraw_history'
    '''
    table_order_task = TinyDB('database/twh_order_task.json')
    table_withdraw_history = TinyDB('database/twh_withdraw_history.json')


    @classmethod
    def get_fullfilled_shipout_box_id(cls, user_id) -> int:
        q = Query()
        s = cls.table_order_task.search((q.user_id == user_id) & (q.state=='fullfilled'))
        if len(s) > 0:
            return s[0]['connected_box_id']
        return 0

    @classmethod
    def Create_OrderTasks_multi_rows(cls, request):
        '''
        request:  a diction discribes order.  multi-element of request['location_**'] 
        This function will convert the request into multi lines.
        '''
        Logger.Debug("DB_Order_task:: Create_OrderTasks_multi_rows()")
        for key, value in request.items():
            if key[0:9] == 'location_':
                order_item = {}
                # get location detail.
                order_item['location'] = value  # fullname:  dental_location
                order_item['row'] = get_row_from_tooth_location(value)
                order_item['col'] = db_Stock.get_col_id(request, value)
                order_item['layer'] = int(value[3])
                # Logger.Print('location_', value)
                # Logger.Print('row', order_item['row'])
                # Logger.Print('col', order_item['col'])
                # Logger.Print('layer', order_item['layer'])

                # copy from request, descript tooth
                order_item['located'] = request['located']  # -3: porter,  -2: worker_hand  0..11  packer_cell

                #copy from request, descript order
                order_item['order_state'] = request['order_state']  # 'idle', 'feeding', 'fullfiled' , 'packed',  can be virtual.
                order_item['linked_packer_cell_id'] = request['linked_packer_cell_id']  # int (0:11)

                order_item['order_id'] = request['order_id']
                order_item['user_id'] = request['user_id']
                order_item['brand'] = request['brand']
                order_item['batch_number'] = request['batch_number']
                order_item['color'] = request['color']
                order_item['shape'] = request['shape']
                order_item['size'] = request['size']
                new_doc_id = cls.table_order_task.insert(order_item)
                # Logger.Print('new_doc_id ', new_doc_id)

    @classmethod
    def link_to_packer_cell(cls, order_id:str) -> int:
        '''
        # Instruction
        1. (by caller) confirm the order state is idle .
        2. try to find an empty packer_cell. If failed, return -1
        3. update table_order_task.linked_packer_cell , return packer_cell_id
        '''
        q = Query()
        assigned_pack_cell_ids =  cls.table_order_task.search(q.linked_packer_cell_id != order_id)

        # https://stackoverflow.com/questions/3462143/get-difference-between-two-lists-with-unique-entries
        all_packer_cell_ids = [0,1,2,3,4,5,6,7,8,9,10,11]
        empty_packer_cell_ids = set(all_packer_cell_ids) - set(assigned_pack_cell_ids)

        if (len(empty_packer_cell_ids)) == 0:
            return -1
        
        target_packer_cell_id = empty_packer_cell_ids[0]['linked_packer_cell_id']
        cls.table_order_task.update()
        return target_packer_cell_id
    

    @classmethod
    def remove_tooth_from_withdraw_quieue(cls, order_id:int, location:str) -> bool:
        cls.table_order_task.remove(where(('location')==location) &('order_id')==order_id)
        q = Query()
        rows = cls.table_order_task.search(q.order_id == order_id)
        if len(rows) == 0:
            # all teeth have been removed to shipout_box already
            return True
        # still has other tooth to be removed.
        return False
    
    @classmethod
    def get_single_order(cls) -> list:
        # A. get first row's order_id
        # B. get a list for all teeth with this order_id
        all = cls.table_order_task.all()
        if len(all) > 0:
            order_id = all[0]['order_id']
            q = Query()
            s = cls.table_order_task.search(q.order_id==order_id)
            return s
        return []

    @classmethod
    def update_tooth_located(cls, doc_id:int, located:str):
        doc_ids = [doc_id]
        # print("stock_rule_update()", rule_item)
        item = {}
        item['located'] = located
        cls.table_order_task.update(item, doc_ids=doc_ids)

    @classmethod
    def update_order_state(cls, new_state:str, doc_ids):
        item = {}
        item['order_state'] = new_state
        cls.table_order_task.update(item, doc_ids= doc_ids)

    @classmethod
    def delete_by_order_id(cls, order_id):
        # cls.table_deposit_history.remove(where ('doc_id')== doc_id)
        cls.table_order_task.remove(where ('order_id') == order_id)
# class DbShipout():
#     table_takeout = TinyDB('twh_takeout.json')

#     @classmethod
#     def init_table(cls):
#         content = {}
#         content['request_user_id'] = 'abc'
#         boxes = []
#         for i in range(12):
#             box={}
#             box['id'] = i
#             box['order_id'] = i
#             # box['state'] = 'idle'
#             boxes.append(box)
#         content['boxes'] = boxes
#         cls.table_takeout.insert(content)


#     @classmethod
#     def get_fullfilled_packer_cell_id(cls, user_id:str) -> int:
#         q= Query()
#         Logger.Debug("DbShipout::get_shipout_box_id()")
#         Logger.Print('user_id', user_id)
#         s = cls.table_takeout.search(q.user_id == user_id)
#         if len(s) > 0:
#             return s[0]['user_id']
#         return -1

#     @classmethod
#     def Update_shipout_request(cls, user_id:str):
#         '''
#         TODO:  be a history in database
#         '''
#         content= cls.table_takeout.all()
#         content['request_user_id'] = user_id
#         cls.table_takeout.update(content)

    