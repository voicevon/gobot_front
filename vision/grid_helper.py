from vision.grid_finder import GridFinder


class Grid_Helper():
    '''
    Find grid perspective view from origin image
    Find stones and come out layout
    '''
    def __init__(self, grid_config, cell_config):
        self.__grid_finder = GridFinder(grid_config)

    def scan_layout(self, origin_image):
        layout = None
        image = self.__grid_finder.auto_perspect(origin_image)
        if image is None:
            return None
        

        return layout

    