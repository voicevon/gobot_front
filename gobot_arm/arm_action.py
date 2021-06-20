class gobot_arm():
    '''
    Current Position -> Move_up 
    -> move to pickup_point -> Move down -> Suck on -> Move up 
    -> Move to placedown_point -> [lower_release] Move_Down -> Suck Off -> Move_up
    -> [if do_parking] Move to park_point (End)
    '''
    
    action_code = 1
    class pickup_at:
        x = 1
        y = 1
    class placedown_at:
        lower_release = True
        x = 2
        y = 2
    class park_at:
        do_parking = True
        x = 3
        y = 3 

    def __init__(self):
        self.__last_action = 0

    def do(self, action_code):
        '''
        action == 0: do nothing. 
                        Setting by head
                == 1: do nothing
                        Setting by arm
        action == 2: pickup and place , and parking
        action == 4: pickup and place
        action == 6: park directly
        action == 8: home_x
        action == 10: home_y
        action == 12: query arm status
        '''

        # put action command to BLE buffer
        ble_write(action_code)

