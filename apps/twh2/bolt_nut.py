from logger import Logger

twh_factory = {'221109':'山东雅乐福义齿公司'}



def get_row_from_tooth_location(location_string:str) -> int:
    print(location_string)
    if location_string[0:2] == 'ul':
        return 1
    elif location_string[0:2] == 'ur':
        return 2
    elif location_string[0:2] == 'll':
        return 0
    elif location_string[0:2] == 'lr':
        return 3



class PickingPacking_Tooth():
    # def __init__(self, order_id:int, shipoutbox_id:int, row:int, col:int, layer:int) -> None:
        # self.order_id = order_id
        # self.shipoutbox_id = shipoutbox_id
        # self.row =  row
        # self.col = col
        # self.layer = layer

    def __init__(self, dbtable_withdraw_queue) -> None:
        if dbtable_withdraw_queue is not None:
            self.order_id = dbtable_withdraw_queue['order_id']
            self.packbox_id = dbtable_withdraw_queue['connected_shipout_box']  #TODO:  unify the key's name
            self.row =  dbtable_withdraw_queue['row']
            self.col = dbtable_withdraw_queue['col']
            self.layer = dbtable_withdraw_queue['layer']

    def print_out(self, title):
        Logger.Debug(title)
        Logger.Print(' Tooth order_id\t' , self.order_id)
        Logger.Print(' row=]\t' , self.row)
        Logger.Print(' col=\t' , self.col)
        Logger.Print(' layer=\t ' , self.layer)
        Logger.Print(' packbox_id=\t' , self.packbox_id)
