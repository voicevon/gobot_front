
# import numpy
import json
from mes_elements import MapElement_AGV, MapElement_Node, MyJsonEncoder

class MES_ResourcesHelper:
    '''
    RfCard_id      MainRoad_OnLeft    StationType,   Station_id
    1                   True            SHORT_CUT_ONLY   NULL
    2                   True            LOAD_ONLY        101
    3                   False           SHORT_CUT_ONLY   NULL
    4                   True            LOAD_UNLOAD      203
    5                   True            CHARGE           908
    '''
    def __init__(self) -> None:
        pass

    def GetNode(self, node_id) -> MapElement_Node:
        return self.data

    def AppendMapElement(self, new_node:MapElement_Node) ->None:
        self.all_map_nodes.append(new_node)
        JSONData = json.dumps(self.all_map_nodes, indent=4, cls=MyJsonEncoder)
        print(JSONData)
        textfile = open(self.file_name_map, "w")
        textfile.write(JSONData)
        textfile.close()

    def AppendAgv(self, new_agv:MapElement_AGV) ->None:
        self.all_agvs.append(new_agv)
        JSONData = json.dumps(self.all_agvs, indent=4, cls=MyJsonEncoder)
        print(JSONData)
        textfile = open(self.file_name_agvs, "w")
        textfile.write(JSONData)
        textfile.close()

def init_json_files():
    nn = MapElement_Node()
    nn.Node_id = 111
    nn.SationType = 222
    nn.Station_id = 333
    nn.MainRoad_IsonLeft = True

    resources.AppendMapElement(nn)
    nn.Node_id = 123
    resources.AppendMapElement(nn)

    agv=MapElement_AGV(4444)
    resources.AppendAgv(agv)


if __name__ == '__main__':
    resources = MES_ResourcesHelper()
    # init_json_files()


    



