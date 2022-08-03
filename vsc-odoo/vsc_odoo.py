import xmlrpc.client
# https://www.odoo.com/documentation/15.0/developer/api/external_api.html#connection
class OdooBasic():
    def TestDatabase(self):
        # Test database
        info = xmlrpc.client.ServerProxy('https://demo.odoo.com/start').start()
        url, db, username, password = info['host'], info['database'], info['user'], info['password']
        self.url = info['host']
        print('Test result:')
        print(url, db, username, password)
    
    def ConfigConnection(self):
        self.url = 'http://voicevon.vicp.io:8069'
        self.db = 'odoo'
        self.username = 'voicevon@gmail.com'
        self.password = 'e4d3b344e9d07bf27c9e99f3082fa89a5039d896'

    def Login(self):
        common = xmlrpc.client.ServerProxy('{}/xmlrpc/2/common'.format(self.url))
        self.uid = common.authenticate(self.db, self.username, self.password, {})
        self.models = xmlrpc.client.ServerProxy('{}/xmlrpc/2/object'.format(self.url))

        print('----------------------------------------------------')

        print()
        print('server version:')
        print(common.version())

    def Test_execute_kw(self):
        print('check_access_rights  ----------------------------------------------------')
        x =self.models.execute_kw(self.db, self.uid, self.password, 'res.partner', 'check_access_rights', ['read'], {'raise_exception': False})
        print(x)

        print('search ---------------------------------------------------')
        x = self.models.execute_kw(self.db, self.uid, self.password, 'res.partner', 'search', [[['is_company', '=', True]]])
        print(x)

        print('search_count ---------------------------------------------------')
        x = self.models.execute_kw(self.db, self.uid, self.password, 'res.partner', 'search_count', [[['is_company', '=', True]]])
        print(x)

        print('search ver2 ---------------------------------------------------')
        ids = self.models.execute_kw(self.db, self.uid, self.password, 'res.partner', 'search', [[['is_company', '=', True]]], {'limit': 1})
        [record] = self.models.execute_kw(self.db, self.uid, self.password, 'res.partner', 'read', [ids])
        # count the number of fields fetched by default
        x = len(record)
        print (x)

        print('read some fields ---------------------------------------------------')
        x = self.models.execute_kw(self.db, self.uid, self.password, 'res.partner', 'read', [ids], {'fields': ['name', 'country_id', 'comment']})
        print(x)

        print('fields_get ---------------------------------------------------')
        x = self.models.execute_kw(self.db, self.uid, self.password, 'res.partner', 'fields_get', [], {'attributes': ['string', 'help', 'type']})
        print(x)

        print('search_read ---------------------------------------------------')
        x = self.models.execute_kw(self.db, self.uid, self.password, 'res.partner', 'search_read', [[['is_company', '=', True]]], {'fields': ['name', 'country_id', 'comment'], 'limit': 5})
        print(x)

    def Test_insert_update_delete(self):
        id = self.models.execute_kw(self.db, self.uid, self.password, 'res.partner', 'create', [{'name': "New Partner"}])

        self.models.execute_kw(self.db, self.uid, self.password, 'res.partner', 'write', [[id], {'name': "Newer partner"}])
        # get record name after having changed it
        self.models.execute_kw(self.db, self.uid, self.password, 'res.partner', 'name_get', [[id]])

        self.models.execute_kw(self.db, self.uid, self.password, 'res.partner', 'unlink', [[id]])
        # check if the deleted record is still in the database
        self.models.execute_kw(self.db, self.uid, self.password, 'res.partner', 'search', [[['id', '=', id]]])

    def GetLocation(self):
        model_name = 'stock.location'
        # ids = self.models.execute_kw(self.db, self.uid, self.password, model_name, 'search', [[]], {'limit': 200})
        ids = self.models.execute_kw(self.db, self.uid, self.password, model_name, 'search', [[['name','ilike','J3']]], {'limit': 20})
        print("================================================")
        print(ids)
        for id in ids:
            print('---------------------------------------------------')
            [record] = self.models.execute_kw(self.db, self.uid, self.password, model_name, 'read', [id])
            print(record['location_id'], record['name'], record['child_ids'])
            # print(record)

    def GetProduct(self):
        model_name = 'product.product'
        ids = self.models.execute_kw(self.db, self.uid, self.password, model_name, 'search', [[]], {'limit': 8})
        # ids = self.models.execute_kw(self.db, self.uid, self.password, model_name, 'search', [[['name','ilike','J3']]], {'limit': 20})
        print("================================================")
        print(ids)
        for id in ids:
            print('---------------------------------------------------')
            [record] = self.models.execute_kw(self.db, self.uid, self.password, model_name, 'read', [id])
            print(record['name'],record['description'])

    def GetStockQuantity(self):
        model_name = 'stock.quant'
        # ids = self.models.execute_kw(self.db, self.uid, self.password, model_name, 'search', [[]], {'limit': 8})
        ids = self.models.execute_kw(self.db, self.uid, self.password, model_name, 'search', [[['name','ilike','J3']]], {'limit': 20})
        print("================================================")
        print(ids)
        for id in ids:
            print('---------------------------------------------------')
            [record] = self.models.execute_kw(self.db, self.uid, self.password, model_name, 'read', [id])
            location_id, location_name = record['location_id']
            product_id, product_name = record['product_id']
            print(location_name,product_name)
            # print(record['product_id'],record['location_id'], record['quantity'])
            # print(record)





if __name__ == "__main__":
    # print("Hello")
    odoo = OdooBasic()
    # odoo.TestDatabase()
    odoo.ConfigConnection()
    odoo.Login()
    # odoo.Test_execute_kw()
    # odoo.GetLocation()
    odoo.GetProduct()
    # odoo.GetStockQuantity()

