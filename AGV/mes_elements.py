import sys
sys.path.append('C:\\gitlab\\gobot_front')  # For runing in VsCode on Windows-10 


import json
from json import JSONEncoder
from collections import namedtuple

class FileNames:
    MapAgvs = 'config_agvs.json'
    MapNodes = 'config_map_nodes.json'

class Single_MesTask:
    def __init__(self) -> None:
        self.load_from = MapElement_Node()
        self.unload_to = MapElement_Node()
        self.state = 0


class MapElement_Robot:
    # after hardware reseting,  AGV will move forward, 
    # after detecting the first location mark, will go to branch and goto idle.
    def __init__(self, id:int) -> None:
        self.id = id
        self.location = -1   # means unknown.
        self.state = -1   # Charging, moving to load, loading, moving to unload, unloading, idle.
        self.battery_voltate = 12.0

    def on_mqtt_message(self, location_id, battery_voltage, state):
        self.location =  location_id
        self.battery_voltate = battery_voltage
        self.State =  state


class MapElement_Node:
    # def __init__(self) -> None:
    Node_id = 1   # Only on main_road
    MainRoad_IsonLeft = True
    Next_Node_id = 2 # Only on main_road
    SationType = 1   # SHORT_CUT_ONLY, No station
    Next_Node_id_Shortcut = 4 # Only for short cut.  Short cut will always from main_road_node to another main_road_node
    Station_id = 123  # Only on brached_road
    StationType = 1  


class MyJsonEncoder(JSONEncoder):
    '''
    https://pynative.com/make-python-class-json-serializable/
    '''
    def default(self, o):
            return o.__dict__


def customStudentDecoder(studentDict):
    '''
    https://pynative.com/python-convert-json-data-into-custom-python-object/
    '''
    return namedtuple('X', studentDict.keys())(*studentDict.values())
