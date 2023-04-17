from tinydb import TinyDB, Query, where
from database.bolt_nut import get_row_from_tooth_location
from database.db_stock import db_Stock

from logger import Logger


class DB_WithdrawOrder():
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

    About order_State
        'idle',        default value when created.
        'feeding',     after linked to a packer-cell
        'fullfilled',  when all teeth moved to packer-cell. 
        'shipping'  ,
        'shipped',     when bule buton is pressed. saying:  set by wcs, delete by wms ??
                       when deleting, will insert into 'table_withdraw_history'
    '''
    table_withdraw_order = TinyDB('database/twh_withdraw_order.json')
    # table_shipping_order = TinyDB('database/twh_shipping_order.json')
    table_withdraw_history = TinyDB('database/twh_withdraw_history.json')


    @classmethod
    def get_fullfilled_orders_by_user_id(cls, user_id) -> list:
        cls.table_withdraw_order.clear_cache()
        q = Query()
        s = cls.table_withdraw_order.search((q.user_id == user_id) & (q.order_state =='fullfilled'))
        return s
    

    #TODO:  How to combine the below two functions ?
    @classmethod
    def get_wms_shipping_orders(cls, twh_id):
        '''
        wms_shipping:   user wanted to ship
        wcs_shipping:   wcs started to ship
        '''
        cls.table_withdraw_order.clear_cache()
        q = Query()
        # s = cls.table_withdraw_order.search((q.twh_id==twh_id) & (q.order_state in ['wms_shipping','wcs_shipping']) )
        s = cls.table_withdraw_order.search((q.twh_id==twh_id) & (q.order_state =='wms_shipping') )
        return s

    @classmethod
    def get_wcs_shipping_orders(cls, twh_id):
        '''
        wms_shipping:   user wanted to ship
        wcs_shipping:   wcs started to ship
        '''
        cls.table_withdraw_order.clear_cache()
        q = Query()
        # s = cls.table_withdraw_order.search((q.twh_id==twh_id) & (q.order_state in ['wms_shipping','wcs_shipping']) )
        s = cls.table_withdraw_order.search((q.twh_id==twh_id) & (q.order_state =='wcs_shipping') )
        return s

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
                order_item['order_state'] = request['order_state']  # 'idle', 'feeding', 'fullfiled' , 'shipping', 'shipped'
                order_item['linked_packer_cell_id'] = request['linked_packer_cell_id']  # int (0:11)
                order_item['order_id'] = request['order_id']
                order_item['order_code'] = request['order_code']  # User's order_id
                order_item['user_id'] = request['user_id']
                order_item['twh_id'] = request['twh_id']
                order_item['brand'] = request['brand']
                order_item['batch_number'] = request['batch_number']
                order_item['color'] = request['color']
                order_item['shape'] = request['shape']
                order_item['size'] = request['size']
                new_doc_id = cls.table_withdraw_order.insert(order_item)
                # Logger.Print('new_doc_id ', new_doc_id)
    
    @classmethod
    def update_order_state(cls, new_state:str, doc_ids):
        item = {}
        item['order_state'] = new_state
        cls.table_withdraw_order.clear_cache()
        cls.table_withdraw_order.update(item, doc_ids= doc_ids)
        cls.table_withdraw_order.clear_cache()


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
    def update_tooth_located(cls, doc_id:int, located:str):
        doc_ids = [doc_id]
        # print("stock_rule_update()", rule_item)
        item = {}
        item['located'] = located
        cls.table_withdraw_order.update(item, doc_ids=doc_ids)


    @classmethod
    def delete_by_order_id(cls, order_id):
        # cls.table_deposit_history.remove(where ('doc_id')== doc_id)
        cls.table_withdraw_order.remove(where ('order_id') == order_id)

    