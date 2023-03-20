from logger import Logger

twh_factory = {'221109':'山东雅乐福义齿公司','230220':'山东郑玉昌义齿公司'}

def get_twh_factory(twh_id:str):
    result = {}
    result['id'] = twh_id
    result['factory_name'] = twh_factory[twh_id]
    return result


def get_row_from_tooth_location(location_string:str) -> int:
    # print(location_string)
    if location_string[0:2] == 'ul':
        return 1
    elif location_string[0:2] == 'ur':
        return 2
    elif location_string[0:2] == 'll':
        return 0
    elif location_string[0:2] == 'lr':
        return 3
    Logger.Error('bolt_nut.py   get_row_from_tooth_location()')
    Logger.Print('location_string', location_string)



class PickingPacking_Tooth():
    def __init__(self, dbtable_withdraw_queue) -> None:
        if dbtable_withdraw_queue is not None:
            self.order_id = dbtable_withdraw_queue['order_id']
            # self.pack_cell_id = dbtable_withdraw_queue['connected_shipout_box']  #TODO:  unify the key's name
            self.pack_cell_id = dbtable_withdraw_queue['linked_packer_cell_id']  #TODO:  unify the key's name
            self.row =  dbtable_withdraw_queue['row']
            self.col = dbtable_withdraw_queue['col']
            self.layer = dbtable_withdraw_queue['layer']
            self.user_id = dbtable_withdraw_queue['user_id']['user_id']

    def ToJson(self):
        json = {}
        json['order_id'] = self.order_id
        json['pack_cell_id'] = self.pack_cell_id
        json['row'] = self.row
        json['col'] = self.col
        json['layer'] = self.layer
        json['user_id'] = self.user_id


    def print_out(self, title):
        Logger.Debug(title)
        Logger.Print(' Tooth order_id\t' , self.order_id)
        Logger.Print(' row' , self.row)
        Logger.Print(' col' , self.col)
        Logger.Print(' layer ' , self.layer)
        Logger.Print(' pack_cell_id' , self.pack_cell_id)
