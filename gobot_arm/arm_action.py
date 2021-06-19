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
        x = 2
        y = 2
    class park_at:
        x = 3
        y = 3 
    lower_release = True
    do_parking = True
    arm_status = False

    def __init__(self):
        self.__last_action = 0

    def do(self, action):
        '''
        action == 0: do nothing
        action == 1: pickup and place , and parking 
        action == 2: pickup and place
        action == 3: park directly
        action == 7: home_x
        action == 8: home_y
        action == 9: query arm status
        '''

        # put action command to BLE buffer
        ble_write(action)
        # https://www.youtube.com/watch?v=olU8ICQgG94

