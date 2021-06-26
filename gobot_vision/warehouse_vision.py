
class finder_config:

    name='WAREHOUSE_ARUCO_CONFIG'
    top_right_id =  13
    bottom_right_id = 34
    bottom_left_id = 21
    top_left_id = 15
    perspected_width = 400 + 1800  # Effect the right edge
    perspected_height = 400 + 50   # effect the bottom edge
    crop_x0 = 50   # effect the left edge
    crop_y0 = 50   # effect the top edge.
    crop_width = 2228   # Don't modify this value
    crop_height = 428  # Don't modify this value.
    
    # aruco_ids = [1, 2, 3, 4]  # [topleft, topright, bottomright, bottomleft]
    # area_scales = [1.1, 1.1, 2.2, 2.2]
    # area_size = (200,900)    # for pespectived view image.

class WarehouseVision():
    def __init(self):
        self.layout = [([0] * 30) for i in range(15)]

    def create_finder_config(self):
        return finder_config
