import enum

from Pylib.robot_map import MapSite

class HouseMapSite_Catalog(enum.Enum):
    ROOM = 1,
    DOOR = 2,
    HEAD = 3,
    NECK = 4,
    PARKING = 5, 

class HouseMapSites():
    def __init__(self) -> None:
        self.head = MapSite('Head', 148.93, 0)
        self.neck = MapSite('Neck', 55.0, 0)
        self.parking = MapSite('Parking', 55.0, -2)

        self.doors = [MapSite]
        self.doors.append (MapSite('Door_0',-10.37,  59.07))
        self.doors.append (MapSite('Door_1',-21.39, 56.12))
        self.doors.append (MapSite('Door_2',-44.28, 40.67))
        self.doors.append (MapSite('Door_3',-57.42, 17.95))
        self.doors.append (MapSite('Door_4',-57.42, -17.95))
        self.doors.append (MapSite('Door_5',-44.28, -40.67))
        self.doors.append (MapSite('Door_6',-21.39, -56.12))
        self.doors.append (MapSite('Door_7',-10.37, -59.07))
        
        self.rooms = [MapSite]
        self.rooms.append(MapSite('Room_0', -78.79, 126.38))
        self.rooms.append(MapSite('Room_1', -98.18, 95.3))
        self.rooms.append(MapSite('Room_2', -111.9, 61.4))
        self.rooms.append(MapSite('Room_3', -119.51, 25.59))
        self.rooms.append(MapSite('Room_4', -119.51, -25.59))
        self.rooms.append(MapSite('Room_5', -111.9, -61.4))
        self.rooms.append(MapSite('Room_6', -98.18, -95.2))
        self.rooms.append(MapSite('Room_7', -78.79, -126.38))

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


