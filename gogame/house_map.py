import enum

from collections import namedtuple
from logging import exception
MapSite = namedtuple("MapSite",['Name','X','Y'])

class HouseMapSite_Catalog(enum.Enum):
    ROOM = 1,
    DOOR = 2,
    HEAD = 3,
    NECK = 4,
    PARKING = 5, 

class HouseMapSites():
    def __init__(self) -> None:
        self.head = MapSite('Head', 20,30)
        self.neck = MapSite('Neck',123,234)
        self.parking = MapSite('Parking',22,34)

        self.doors = [MapSite]
        self.doors.append (MapSite('Door_0',122,333))
        self.doors.append (MapSite('Door_1',122,333))
        self.doors.append (MapSite('Door_2',122,333))
        self.doors.append (MapSite('Door_3',122,333))
        self.doors.append (MapSite('Door_4',122,333))
        self.doors.append (MapSite('Door_5',122,333))
        self.doors.append (MapSite('Door_6',122,333))
        self.doors.append (MapSite('Door_7',122,333))
        
        self.rooms = [MapSite]
        self.rooms.append(MapSite('Room_0', 22,33))
        self.rooms.append(MapSite('Room_1', 22,33))
        self.rooms.append(MapSite('Room_2', 22,33))
        self.rooms.append(MapSite('Room_3', 22,33))
        self.rooms.append(MapSite('Room_4', 22,33))
        self.rooms.append(MapSite('Room_5', 22,33))
        self.rooms.append(MapSite('Room_6', 22,33))
        self.rooms.append(MapSite('Room_7', 22,33))

    def GetSingleSite(self, cat:HouseMapSite_Catalog, index:int) -> MapSite:
        if cat == HouseMapSite_Catalog.HEAD:
            return self.head
        elif cat == HouseMapSite_Catalog.NECK:
            return self.neck
        elif cat == HouseMapSite_Catalog.DOOR:
            return self.doors[index]
        elif cat == HouseMapSite_Catalog.ROOM:
            return self.rooms[index]
        elif cat == HouseMapSite_Catalog.PARKING:
            return self.parking
        else:
            print("[Error] HouseMapSites.GetSingleSite()  cat=, index= ", cat, index)




class HouseMapDiction():

    def __init__(self) -> None:
        self.NECK = (55.0, 0)
        self.HEAD = (148.93, 0)
        self.PARKING = (55.0, -2)

        self.DOORS = [(-10.37,  59.07),
                (-21.39, 56.12),
                (-44.28, 40.67),
                (-57.42, 17.95),
                (-57.42, -17.95),
                (-44.28, -40.67),
                (-21.39, -56.12),
                (-10.37, -59.07),
                ]

        self.ROOMS  = [(-78.79,  126.38),  #0
                (-98.18, 95.3),              #1
                (-111.9, 61.4),              #2
                (-119.51, 25.59),            #3
                (-119.51, -25.59),           #4
                (-111.9, -61.4),             #5
                (-98.18, -95.2),             #6
                (-78.79, -126.38)            #7
                ]

