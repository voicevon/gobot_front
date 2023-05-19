from twh_wcs.twhwcs_common.twh_robot_loop_porter import Twh_LoopPorter
from twh_wcs.von.wcs.conveyor.tube_conveyor import TubeConveyor

g_subsystem_porters = list[Twh_LoopPorter]()
g_subsystem_tube_coneyor = TubeConveyor('221109', 1,'gcode_topic', 'state_topc')