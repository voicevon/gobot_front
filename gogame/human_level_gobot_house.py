
	# House                Y+
	# 					 ^
	# 		 r0          |                                                                     \ beta  
	# 	  r1     d0      |                                                                    \  /
	# 	r2      d1       |                                                                     \/      
	#   r3       d3  d2    |                                                                   /  alpha
	#   -----------------(0,0)------Neck----------Head    --> X+                               --------------------------------->  Alpha,Beta (0,0)
	#   r4      d4  d5     |
	#    r5       d6       |
	# 	 r6     d7       |
	# 	  r7             |





#   House_Vendor_State
#                <----------------------------- FAILED
#               |                                 ^
#               |                                 |
#  UNKNOWN--> BLANK  --> SHIPPING --> I_AM_READY  --> CONFIRMED_READY
#               ^                                          ^  
#               |                                          |
#

import sys
sys.path.append('D:\\XumingSource\\gobot_front')  # For runing in VsCode on Windows-10 
from gogame.human_level_robot_base import HumanLevelRobotBase
from gogame.chessboard_cell import StoneColor
from Pylib.rabbit_mq_helper import g_amq,AMQ_ConnectionConfig
from datetime import datetime
from gogame.house_map import HouseMapSite_Catalog, HouseMapSiteFactory, MapSite

import enum


class HumanLevelHouse_EEF_ACTIONS(enum.Enum):
    LOAD = 3,
    UNLOAD = 4,
    SLEEP = 5,

class HumanLevelHouseState(enum.Enum):
    UNKNOWN = 1
    EMPTY = 2
    SHIPPING = 3
    I_AM_READY = 4
    FAILED = 5
    CONFIRMED_READY = 6

class HumanLevelGobotHouse(HumanLevelRobotBase):

    def __init__(self, robot_serial_id:int, do_home=True) -> None:
        # super().__init__(rabbit_client=rabbit_client)
        super().__init__()
        mq_name = 'gobot_xnnnn_house'
        self.mq_name = mq_name.replace('nnnn', str(robot_serial_id))
        if do_home:
            self.HomeBetaAlpha()
        self.State = HumanLevelHouseState.UNKNOWN
        self.shipping_begin_timestamp = datetime.now()

    def FeedVenderVision(self, stone_color: StoneColor):
        if self.State == HumanLevelHouseState.UNKNOWN:
            if stone_color == StoneColor.BLANK:
                self.State = HumanLevelHouseState.EMPTY
                print('[Info] HumanLevelGobotHouse  FeedVenderVision() to ', self.State)
            if stone_color == stone_color.WHITE:
                self.State = HumanLevelHouseState.CONFIRMED_READY
                print('[Info] HumanLevelGobotHouse  FeedVenderVision() to ', self.State)

        elif self.State == HumanLevelHouseState.EMPTY:
            index = 0
            self.PickupFrom(HouseMapSite_Catalog.ROOM, index)
            self.MoveTo(HouseMapSite_Catalog.DOOR,index)
            self.MoveTo(HouseMapSite_Catalog.NECK)
            self.PlaceTo(HouseMapSite_Catalog.HEAD)
            self.State = HumanLevelHouseState.SHIPPING
            print('[Info] HumanLevelGobotHouse  FeedVenderVision() to ', self.State)
            self.shipping_begin_timestamp = datetime.now()

        elif self.State == HumanLevelHouseState.SHIPPING:
            how_long = datetime.now() - self.shipping_begin_timestamp
            if how_long.total_seconds() > 30:
                self.State = HumanLevelHouseState.I_AM_READY
                print('[Info] HumanLevelGobotHouse  FeedVenderVision() to ', self.State)

        elif self.State == HumanLevelHouseState.I_AM_READY:
            if stone_color == StoneColor.BLANK:
                self.State = HumanLevelHouseState.FAILED
                print('[Info] HumanLevelGobotHouse  FeedVenderVision() to ', self.State)
            if stone_color == StoneColor.WHITE:
                self.State = HumanLevelHouseState.CONFIRMED_READY
                print('[Info] HumanLevelGobotHouse  FeedVenderVision() to ', self.State)

        elif self.State == HumanLevelHouseState.CONFIRMED_READY:
            if stone_color == StoneColor.BLANK:
                self.State = HumanLevelHouseState.EMPTY
                print('[Info] HumanLevelGobotHouse  FeedVenderVision() to ', self.State)
            
    def HomeBetaAlpha(self):
        commands = ['G28BI', 'G28AI', 'M996']
        g_amq.PublishBatch(self.mq_name, commands)
        
    def __Pickup_Place(self, from_where:MapSite, to_where:MapSite, auto_park=False):
        self.__PickupFrom(from_where)
        self.__PlaceTo(to_where)
        if auto_park:
            parking_at = HouseMapSiteFactory().MakeSingleSite(HouseMapSite_Catalog.PARKING)
            self.__MoveTo(parking_at)
        g_amq.Publish(self.mq_name, 'M996')

    def Pickup_Place(self, from_cat:HouseMapSite_Catalog, from_index:int, to_cat:HouseMapSite_Catalog, to_index:int=0):
        from_site = HouseMapSiteFactory().MakeSingleSite(from_cat, from_index)
        to_site = HouseMapSiteFactory().MakeSingleSite(to_cat, to_index)
        self.__Pickup_Place(from_site, to_site)


    def __MoveTo(self, the_site:MapSite):
        gcode = 'G1X' + str(the_site.X) + 'Y' + str(the_site.Y)
        g_amq.Publish(self.mq_name, gcode)

    def MoveTo(self, cat:HouseMapSite_Catalog, index:int=0):
        the_site = HouseMapSiteFactory().MakeSingleSite(cat, index)
        self.__MoveTo(the_site)

    def DisableMotor(self):
        g_amq.Publish(self.mq_name, 'M84')

    def __PickupFrom(self, the_site: MapSite):
        self.__MoveTo(the_site)
        self.EefAction(HumanLevelHouse_EEF_ACTIONS.LOAD)

    def PickupFrom(self, cat:HouseMapSite_Catalog, index:int=0):
        the_site = HouseMapSiteFactory().MakeSingleSite(cat, index)
        self.__PickupFrom(the_site)

    def __PlaceTo(self, site:MapSite):
        self.__MoveTo(site)
    
    def PlaceTo(self, cat:HouseMapSite_Catalog, index:int=0):
        the_site = HouseMapSiteFactory().MakeSingleSite(cat, index)
        self.__PlaceTo(the_site)


    def EefAction(self, eef: HumanLevelHouse_EEF_ACTIONS):
        if eef==HumanLevelHouse_EEF_ACTIONS.LOAD:
            g_amq.Publish(self.mq_name, 'M123P1S3')
        elif eef==HumanLevelHouse_EEF_ACTIONS.UNLOAD:
            g_amq.Publish(self.mq_name, 'M123P1S4')
        elif eef==HumanLevelHouse_EEF_ACTIONS.SLEEP:
            g_amq.Publish(self.mq_name, 'M123P1S5')

    def Test_Eef(self):
        # ('M123P1S3')   #load
        # ('M123P1S4')   #Release
        commands = ['M123P1S3','G4S5','M123P1S4','G4S5','M996' ]
        g_amq.PublishBatch(self.mq_name, commands)



        # self.rabbit_client.PublishToHouse('M123P1S3')   #load
        # self.rabbit_client.PublishToHouse('G4S5')
        # self.rabbit_client.PublishToHouse('M123P1S4')   #Release
        # self.rabbit_client.PublishToHouse('G4S5')
        # self.rabbit_client.PublishToHouse('M996')
        
        # self.rabbit_client.PublishToArm('M123P1S128')
        # self.rabbit_client.PublishToArm('G4S1')
        # self.rabbit_client.PublishToArm('M123P1S0')
        # self.rabbit_client.PublishToArm('G4S5')
        # self.rabbit_client.PublishToArm('M996')

    def demo(self):
        site = HouseMapDiction()
        x,y = site.DOORS[0]
        self.MoveTo(x,y)
        for index in range(8):
            x,y = site.ROOMS[index]
            self.MoveTo(x,y)
            self.EefAction(HumanLevelHouse_EEF_ACTIONS.LOAD)
            x,y = site.DOORS[index]
            self.MoveTo(x,y)
            # self.EefAction(HumanLevelHouse_EEF_ACTIONS.UNLOAD)
        x,y = site.DOORS[7]
        self.MoveTo(x,y)
        self.PreHome()
        g_amq.Publish(self.mq_name, "M84")

    def PreHome(self):
        # Park Arms at a point, nearby homed position
        commands = ['G1B120F2800', 'G1A-1F2800', 'M996', "M84"]
        g_amq.PublishBatch(self.mq_name, commands)

    def Calibrate_home_position_alpha(self):
        pause = 'G4S3'
        commands = ["G28AI", "G1A0",pause,'G1A-30',pause,'G1A-330','M996']
        g_amq.PublishBatch(self.mq_name, commands)

    def Calibrate_home_position_beta(self):
        pause = 'G4S3'
        commands = ['G28AI', 'G1A0',"G28BI", "G1B20",pause,'G1A-30B120','M996']
        g_amq.PublishBatch(self.mq_name, payloads=commands)

        # prehome for next run.
        # house.PreHome()


if __name__ == '__main__':
    config = AMQ_ConnectionConfig()
    g_amq.ConnectToRabbitMq(config)

    house = HumanLevelGobotHouse(robot_serial_id=2134, do_home=False)
    house.Calibrate_home_position_alpha()
    # for i in range(10):
    #     house.Calibrate_home_position_beta()

    # Test top level movemnet
    from_site = HouseMapSiteFactory().MakeSingleSite(HouseMapSite_Catalog.ROOM, 1)
    to_site = HouseMapSiteFactory().MakeSingleSite(HouseMapSite_Catalog.HEAD, 0)
    # house.__Pickup_Place(from_site, to_site)
    house.Pickup_Place(HouseMapSite_Catalog.ROOM,1, HouseMapSite_Catalog.HEAD)