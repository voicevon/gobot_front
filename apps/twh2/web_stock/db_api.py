
from tinydb import TinyDB, Query, where
from bolt_nut import get_row_from_tooth_location

class TwhLocation:
    row = -1
    col = -1 
    layer = -1

class db_Stock():
    table_stock = TinyDB('twh_stock.json')

    @classmethod
    def get_stock(cls, request):  # -> List[Document]
        q = Query()
        db_rows = cls.table_stock.search((q.brand == request['brand']) 
                                        & (q.color == request['color'])
                                        & (q.size == request['size'])
                                        & (q.shape == request['shape'])
                                        & (q.batch_number == request['batch_number'])
                                        & (q.location == request['location'])
                                        )
        if len(db_rows) > 0:
            return db_rows[0]
        return None

    @classmethod
    def check_stock_for_all_locations(cls, request) -> bool:
        # print(request)
        for key, value in request.items():
            if key[0:9] == 'location_':
                q= Query()
                db_rows = cls.table_stock.search((q.brand == request['brand']) 
                                            & (q.color == request['color'])
                                            & (q.size == request['size'])
                                            & (q.shape == request['shape'])
                                            & (q.batch_number == request['batch_number'])
                                            & (q.location == value)
                                            )
                if len(db_rows) == 0:
                    # at least one location has no stock.
                    return False
        return True

    @classmethod
    def get_col_id(cls, request, location) -> int:
        '''
        return -1 if not found.
        '''
        q= Query()
        db_rows = cls.table_stock.search((q.brand == request['brand']) 
                                    & (q.color == request['color'])
                                    & (q.size == request['size'])
                                    & (q.shape == request['shape'])
                                    & (q.batch_number == request['batch_number'])
                                    & (q.location == location)
                                    )
        if len(db_rows) > 0:
            # at least one location has no stock.
            return db_rows[0]['col']
        print('get_col_id()  can not find col_id !    ', location , request )
        return -1

    @classmethod
    def get_pure_empty_col(cls, request):
        # To find a col number,
        # 1.  Try to find same brand, color, size, shape, batch_number as request.
        # 2.  If not #1,  Try to all {row, layer} is empty.
        # 3.  If not #2,  A) return None,  B) Go on with any empty box. ?? 
        q = Query()
        db_rows = cls.table_stock.search((q.brand == request['brand']) 
                                    & (q.color == request['color'])
                                    & (q.size == request['size'])
                                    & (q.shape == request['shape'])
                                    & (q.batch_number == request['batch_number'])
                                    )
        max_col = 0
        for dbrow in db_rows:
            if dbrow['col'] > max_col:
                max_col = dbrow['col']
        return max_col + 1

    @classmethod
    def get_emptybox(cls) -> TwhLocation:
        q = Query()
        # Not found in stock, assign [col]
        box = TwhLocation()
        for layer in range(1):
            for row in range(4):
                for col in range(120):
                    db_rows = cls.table_stock.search((q.layer==str(layer)) & (q.row==str(row)) & (q.col==str(col)))
                    if len(db_rows) == 0:
                        box.layer = layer
                        box.row = row
                        box.col = col
                        # print('get_emptybox', box.layer, box.row, box.col)
                        return box
        # Can not find an empty box
        box.layer = -1
        box.row = -1
        box.col = -1

    @classmethod
    def update_stock(cls, user_request):
        print("color", user_request['color'])
        if user_request['doc_id'] == '-1':
            # insert into database
            print("insert stock")
            db_row={}
            db_row['twh'] = user_request['twh']
            db_row['brand'] = user_request['brand']
            db_row['batch_number'] = user_request['batch_number']
            db_row['color'] = user_request['color']
            db_row['size'] = user_request['size']
            db_row['shape'] = user_request['shape']
            db_row['location'] = user_request['location']
            db_row['layer'] = int(user_request['layer'])
            db_row['row'] = int(user_request['row'])
            db_row['col'] = int(user_request['col'])
            db_row['stock_quantity'] = int(user_request['deposit_quantity'])
            cls.table_stock.insert(db_row)

        else:
            # update into database()
            doc_id = [int(user_request['doc_id'])]
            new_quantity = int(user_request['origin_quantity']) + int(user_request['deposit_quantity'])
            print("update stock", new_quantity)
            cls.table_stock.update({'stock_quantity':new_quantity}, doc_ids=doc_id)

class db_Deposite():
    table_deposit = TinyDB('twh_deposit.json')

    @classmethod 
    def append_deposit(cls, user_request):
        cls.table_deposit.insert(user_request)
    
    @classmethod 
    def search_deposit(cls, robot_id:str, layer_id:str):
        q = Query()
        s = cls.table_deposit.search((q.twh==robot_id) & (q.layer==layer_id))
        return s

    @classmethod 
    def remove_deposit(cls, doc_id:int):
        cls.table_deposit.remove(where ('doc_id')== doc_id)


class db_Withdraw():
    table_withdraw_queue = TinyDB('twh_withdraw_queue.json')
    table_withdraw_history = TinyDB('twh_withdraw_history.json')

    @classmethod
    def get_fullfilled_shipout_box_id(cls, user_id) -> int:
        q = Query()
        s = cls.table_withdraw_queue.search((q.user_id == user_id) & (q.state=='fullfilled'))
        if len(s) > 0:
            return s[0]['connected_box_id']
        return 0

    @classmethod
    def insert_withdraw_queue_multi_rows(self, request):
        for key, value in request.items():
            if key[0:9] == 'location_':
                # print('insert_withdraw_queue_multi_rows()', key, value)
                item = {}
                item['order_id'] = request['order_id']
                item['user_id'] = request['user_id']
                item['brand'] = request['brand']
                item['batch_number'] = request['batch_number']
                item['color'] = request['color']
                item['shape'] = request['shape']
                item['size'] = request['size']
                item['location'] = value

                item['row'] = get_row_from_tooth_location(value)
                item['col'] = db_Stock.get_col_id(request, value)
                item['layer'] = value[3]
                item['connected_shipout_box'] = -1
                self.table_withdraw_queue.insert(item)

    @classmethod
    def update_withdraw_queue_multi_rows_connected_shipout_box(cls, order_id:str, connect_to_shipoutbox_id: int):
        cls.table_withdraw_queue.update()   # syntax

    @classmethod
    def remove_tooth_from_withdraw_quieue(cls, order_id:int, location:str) -> bool:
        cls.table_withdraw_queue.remove(where(('location')==location) &('order_id')==order_id)
        q = Query()
        rows = cls.table_withdraw_queue.search(q.order_id == order_id)
        if len(rows) == 0:
            # all teeth have been removed to shipout_box already
            return True
        # still has other tooth to be removed.
        return False
    
    @classmethod
    def get_single_order(cls) -> list:
        # 1. get first row's order_id
        # 2. get a list for all teeth with this order_id
        all = cls.table_withdraw_queue.all()
        if len(all) > 0:
            order_id = all[0]['order_id']
            q = Query()
            s = cls.table_withdraw_queue.search(q.order_id==order_id)
            return s
        return []


class db_Shipout():
    table_takeout = TinyDB('twh_takeout.json')

    @classmethod
    def init_table(cls):
        content = {}
        content['request_user_id'] = 'abc'
        boxes = []
        for i in range(12):
            box={}
            box['id'] = i
            box['order_id'] = i
            # box['state'] = 'idle'
            boxes.append(box)
        content['boxes'] = boxes
        cls.table_takeout.insert(content)


    @classmethod
    def get_shipout_box_id(cls, user_id:str) -> int:
        q= Query()
        s = cls.table_takeout.search(q.user_id == user_id)
        if len(s) > 0:
            return s[0]['user_id']
        return -1

    @classmethod
    def Update_shipout_request(cls, user_id:str):
        '''
        TODO:  be a history in database
        '''
        content= cls.table_takeout.all()
        content['request_user_id'] = user_id
        cls.table_takeout.update(content)

    

