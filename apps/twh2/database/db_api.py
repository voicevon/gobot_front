
from tinydb import TinyDB, Query, where
from bolt_nut import get_row_from_location

class TwhLocation:
    row = -1
    col = -1 
    layer = -1



class DbApi():
    def __init__(self) -> None:
        self.db_stock = TinyDB('database/twh_stock.json')
        self.db_user = TinyDB('database/twh_user.json')
        self.table_deposit = TinyDB('database/twh_deposit.json')
        self.table_withdraw_queue = TinyDB('database/twh_withdraw_queue.json')
        self.table_withdraw_history = TinyDB('database/twh_withdraw_history.json')
        # self.deposit_request = []


    def get_stock(self, request):  # -> List[Document]
        q = Query()
        db_rows = self.db_stock.search((q.brand == request['brand']) 
                                        & (q.color == request['color'])
                                        & (q.size == request['size'])
                                        & (q.shape == request['shape'])
                                        & (q.batch_number == request['batch_number'])
                                        & (q.location == request['location'])
                                        )
        if len(db_rows) > 0:
            return db_rows[0]
        return None

    def check_stock_for_all_locations(self, request) -> bool:
        print(request)
        for key, value in request.items():
            if key[0:9] == 'location_':
                q= Query()
                db_rows = self.db_stock.search((q.brand == request['brand']) 
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
    def get_pure_empty_col(self, request):
        # To find a col number,
        # 1.  Try to find same brand, color, size, shape, batch_number as request.
        # 2.  If not #1,  Try to all {row, layer} is empty.
        # 3.  If not #2,  A) return None,  B) Go on with any empty box. ?? 
        q = Query()
        db_rows = self.db_stock.search((q.brand == request['brand']) 
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

    def get_emptybox(self) -> TwhLocation:
        q = Query()
        # Not found in stock, assign [col]
        box = TwhLocation()
        for layer in range(1):
            for row in range(4):
                for col in range(120):
                    db_rows = self.db_stock.search((q.layer==str(layer)) & (q.row==str(row)) & (q.col==str(col)))
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

    def update_stock(self, user_request):
        print("color", user_request['color'])
        if user_request['doc_id'] == '-1':
            # insert into database
            print("insert stock")
            db_row={}
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
            self.db_stock.insert(db_row)

        else:
            # update into database()
            doc_id = [int(user_request['doc_id'])]
            new_quantity = int(user_request['origin_quantity']) + int(user_request['deposit_quantity'])
            print("update stock", new_quantity)
            self.db_stock.update({'stock_quantity':new_quantity}, doc_ids=doc_id)

    def get_user(self, user_id):
        q = Query()
        users = self.db_user.search(q.user_id==user_id)
        if len(users) > 0:
            return users[0]
        return None

    def get_user_all(self):
        return self.db_user.all()
        
    def get_stock_all(self):
        return self.db_stock.all()
        
    def append_deposit(self, user_request):
        self.table_deposit.insert(user_request)

    
    def search_deposit(self, robot_id:str, layer_id:str):
        q = Query()
        s = self.table_deposit.search((q.twh==robot_id) & (q.layer==layer_id))
        # print(s)
        return s

    def remove_deposit(self, doc_id:int):
        self.table_deposit.remove(where ('doc_id')== doc_id)
    
    def insert_withdraw_queue_multi_rows(self, request):
        for key, value in request.items():
            if key[0:9] == 'location_':
                print('insert_withdraw_queue_multi_rows()', key, value)
                item = {}
                item['order_id'] = request['order_id']
                item['user_id'] = request['user_id']
                item['brand'] = request['brand']
                item['batch_number'] = request['batch_number']
                item['color'] = request['color']
                item['shape'] = request['shape']
                item['size'] = request['size']
                item['location'] = value

                item['row'] = 1  #TODO:   get correct value.
                item['col'] = 2
                item['layer'] = 3
                item['connected_shipout_box'] = -1
                self.table_withdraw_queue.insert(item)

    def update_withdraw_queue_multi_rows_connected_shipout_box(self, order_id:str, connect_to_shipoutbox_id: int):
        self.table_withdraw_queue.update()   # syntax

    def remove_tooth_from_withdraw_quieue(self, order_id:int, location:str) -> bool:
        self.table_withdraw_queue.remove(where(('location')==location) &('order_id')==order_id)
        q = Query()
        rows = self.table_withdraw_queue.search(q.order_id == order_id)
        if len(rows) == 0:
            # all teeth have been removed to shipout_box already
            return True
        # still has other tooth to be removed.
        return False
    
    def get_fullfilled_shipout_box_id(self, user_id) -> int:
        q = Query()
        s = self.table_withdraw_queue.search((q.user_id == user_id) & (q.state=='fullfilled'))
        if len(s) > 0:
            return s[0]['connected_box_id']
        return 0

    # def take_teeth_from_shipout_box(self, box_id:int):
    #     # discuss: If we delete the record by user_id, might cause multiple records be removed.

    #     #delete from queue
    #     self.table_withdraw_queue.remove(where('connected_box_id') == box_id)
        
g_database = DbApi()

