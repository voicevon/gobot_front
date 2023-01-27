


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

    def print_out(self):
        ss = ' Tooth order_id=' + str(self.order_id)
        ss += ' packbox_id=' + str(self.packbox_id)
        ss += ' row=' + str(self.row)
        ss += ' col=' + str(self.col)
        ss += ' layer= ' + str(self.layer)
        print(ss)