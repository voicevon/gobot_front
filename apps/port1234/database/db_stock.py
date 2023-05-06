
from tinydb import TinyDB, Query, where
from logger import Logger

class TwhLocation:
    row = -1
    col = -1 
    layer = -1

class db_StockRule():
    table_stock_rule = TinyDB('database/twh_stock_rule.json')

    @classmethod
    def get_col_from_request(cls, req)-> TwhLocation:
        q = Query()
        db_result = cls.table_stock_rule.search((q.twh_id==req['twh_id']) 
                                    & (q.brand==req['brand']) 
                                    & (q.batch_number==req['batch_number'])  
                                    & (q.size == req['size'])
                                    & (q.color==req['color']) 
                                    & (q.shape==req['shape']))
        if len(db_result) == 0:
            return None

        ret = TwhLocation()
        ret.row = 1
        # Logger.Print('db col', db_result)
        ret.col = db_result[0]['col']
        ret.layer = 1
        return ret


    @classmethod
    def update(cls, rule_item):
        # cls.table_stock_rule.update()
        doc_id = [int(rule_item['doc_id'])]
        print("stock_rule_update()", rule_item)
        cls.table_stock_rule.update(rule_item, doc_ids=doc_id)

    @classmethod
    def get_all_rules_in_twh(cls, twh_id):
        q= Query()
        ret = cls.table_stock_rule.search(q.twh_id==twh_id)
        return ret

class db_Stock():
    table_stock = TinyDB('database/twh_stock.json')

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
    def get_stock_by_single_brand(cls, selected_brand):  # -> List[Document]
        db_rows = cls.table_stock.search(Query().brand == selected_brand)
        return db_rows
    
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
                                            # & (q.batch_number == request['batch_number'])
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
                                    # & (q.batch_number == request['batch_number'])
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
    def update_rule(cls, rule_item):
        # cls.table_stock_rule.update()
        doc_id = [int(rule_item['doc_id'])]
        # print("stock_rule_update()", rule_item)
        cls.table_stock.update(rule_item, doc_ids=doc_id)

    @classmethod
    def update_quantity(cls, user_request):
        # print("color", user_request['color'])
        if user_request['doc_id'] == '-1':
            # insert into database
            # print("insert stock")
            db_row={}
            db_row['twh_id'] = user_request['twh_id']
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

class db_Deposit_history():
    table_deposit_history = TinyDB('database/twh_deposit_history.json')

    @classmethod 
    def append_deposit(cls, user_request):
        cls.table_deposit_history.insert(user_request)
    
    @classmethod 
    def search_deposit(cls, robot_id:str, layer_id:str):
        q = Query()
        s = cls.table_deposit_history.search((q.twh_id==robot_id) & (q.layer==layer_id))
        return s

    @classmethod 
    def remove_deposit(cls, doc_id:int):
        cls.table_deposit_history.remove(where ('doc_id')== doc_id)

