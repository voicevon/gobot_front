from ble_single_client import BleSingleClient,BleServerHead

class GcodeFactory:
    '''
    Can Help to generate gcode string
    '''
    def __init__(self) -> None:
        self.BlockMovement = False
        self.TargetPosision_or_distance = 0.0
        self.Speed = 1000.0
        self.AxisName = 'X'

    
    def ToString(self) -> str:
        gcode = 'G1'
        if self.BlockMovement:
            gcode = 'G6'
        gcode += self.AxisName + str(self.TargetPosision_or_distance)
        gcode += 'F' + str(self.Speed)
        return gcode



            


