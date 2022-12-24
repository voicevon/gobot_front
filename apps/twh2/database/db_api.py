
from tinydb import TinyDB, Query, where



class TwhLocation:
    layer = -1
    row = -1
    col = -1 



class DbApi():
    def __init__(self) -> None:
        self.db_stock = TinyDB('database/twh_stock.json')
        self.db_user = TinyDB('database/twh_user.json')
        self.table_deposit = TinyDB('database/twh_deposit.json')
        self.table_withdraw = TinyDB('database/twh_withdraw_queue.json')
        # self.deposit_request = []


    def get_stock(self, request):
        q = Query()
        db_rows = self.db_stock.search((q.brand == request['brand']) 
                                        & (q.color == request['color'])
                                        & (q.size == request['size'])
                                        & (q.shape == request['shape'])
                                        & (q.location_vertical == request['location_vertical'])
                                        & (q.location_horizontal == request['location_horizontal'])
                                        & (q.location_index == request['location_index'])
                                        )
        if (len(db_rows) == 0):
            # Can not find in stock
            box = self.get_emptybox()
            request['layer'] = box.layer
            request['row'] = box.row
            request['col'] = box.col
        else:
            # Yes, Find it in stock
            # request['doc_id'] = db_row['doc_id']  #?
            db_row = db_rows[0]
            # print('db_row', db_row.doc_id)
            request['doc_id'] = db_row.doc_id
            request['layer'] = db_row['layer']
            request['row'] = db_row['row']
            request['col'] = db_row['col']
            request['origin_quantity'] = db_row['stock_quantity']
        return request

    def get_emptybox(self) -> TwhLocation:
        q = Query()
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
        if user_request['origin_quantity'] == '0':
            # insert into database
            print("insert stock")
            db_row={}
            db_row['brand'] = user_request['brand']
            db_row['color'] = user_request['color']
            db_row['size'] = user_request['size']
            db_row['shape'] = user_request['shape']
            db_row['location_vertical'] = user_request['location_vertical']
            db_row['location_horizontal'] = user_request['location_horizontal']
            db_row['location_index'] = user_request['location_index']
            db_row['layer'] = (user_request['layer'])
            db_row['row'] = (user_request['row'])
            db_row['col'] = user_request['col']
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

    def append_deposit(self, user_request):
        self.table_deposit.insert(user_request)

    
    def search_deposit(self, robot_id:str, layer_id:str):
        q = Query()
        s = self.table_deposit.search((q.twh==robot_id) & (q.layer==layer_id))
        # print(s)
        return s

    def remove_deposit(self, doc_id:int):
        self.table_deposit.remove(where ('doc_id')== doc_id)
        

g_database = DbApi()

if __name__ == '__main__':
    DbApi().get_emptybox()