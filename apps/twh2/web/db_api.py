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
        db_row = self.db_stock.search(query.color=='白')
        if (len(db_row) == 0):
            # Can not find in stock
            box = self.get_emptybox()
            request['layer'] = box.layer
            request['row'] = box.row
            request['col'] = box.col
        else:
            # Yes, Find it in stock
            request['doc_id'] = db_row['doc_id']  #?
            request['layer'] = db_row['layer']
            request['row'] = db_row['row']
            request['col'] = db_row['col']
            request['origin_quantity'] = db_row['origin_quantity']
        return request

    def get_emptybox(self) -> TwhLocation:
        box = TwhLocation()
        box.layer = 99
        box.row = 98
        box.col = 97
        return box

    def update_stock(self, user_request):
        if user_request['origin_quantity'] == 0:
            # insert into database
            print("insert stock")
            self.db_stock.insert(user_request)
        else:
            # update into database()
            new_quantity = user_request['origin_quantity'] + user_request['deposit_quantity']
            print("update stock")
            # self.db_stock.update()

