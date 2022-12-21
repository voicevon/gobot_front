# https://www.youtube.com/watch?v=aP2KJoTITO0
from tinydb import TinyDB, Query

class UserRequest:
    size = 1
    color = '白'
    layer = 0
    row = 0
    col = 0
    origin_quantity = 0
    deposit_quantity = 0

class TwhLocation:
    layer = 0
    row = 0
    col = 0



class DbApi():
    def __init__(self) -> None:
        self.db_stock = TinyDB('twh_stock.json')
        self.db_user = TinyDB('twh_user.json')
        # self.db.insert({'type': 'OSFY', 'count': 700})
        # self.db.insert({'type': 'EFY', 'count': 800})
        # self.db.all()

    def get_stock(self, request):
        query = Query()
        db_rows = self.db_stock.search(query.color=='白')
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
            print('db_row', db_row.doc_id)
            request['doc_id'] = db_row.doc_id
            request['layer'] = db_row['layer']
            request['row'] = db_row['row']
            request['col'] = db_row['col']
            request['origin_quantity'] = db_row['stock_quantity']
        return request

    def get_emptybox(self) -> TwhLocation:
        box = TwhLocation()
        box.layer = 99
        box.row = 98
        box.col = 97
        return box

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


