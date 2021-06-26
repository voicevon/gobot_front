
class finder_config:

    name='WAREHOUSE_ARUCO_CONFIG'
    top_right_id =  13
    bottom_right_id = 34
    bottom_left_id = 21
    top_left_id = 15

    perspective_width = 450  
    perspective_height = 450 
    top_extended = 100
    right_extended = 100
    bottom_extended = 100
    left_extended = 100

    crop_x0 = 50   # effect the left edge
    crop_y0 = 50   # effect the top edge.
    crop_width = 428   # Don't modify this value
    crop_height = 428  # Don't modify this value.
    
    # aruco_ids = [1, 2, 3, 4]  # [topleft, topright, bottomright, bottomleft]
    # area_scales = [1.1, 1.1, 2.2, 2.2]
    # area_size = (200,900)    # for pespectived view image.

class WarehouseVision():
    def __init__(self):
        self.layout = [([0] * 30) for i in range(15)]

    def create_finder_config(self):
        return finder_config
