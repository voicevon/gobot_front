from logger import Logger


twh_factory = {'221109':'山东雅乐福义齿公司',
                 '230220':'山东郑玉昌义齿公司'}

g_brands = {'沪鸽', '日本松风塑钢','日本山八硬质','日本山八合成'}

def get_twh_factory(twh_id:str):
    result = {}
    result['id'] = twh_id
    result['factory_name'] = twh_factory[twh_id]
    return result


def get_row_from_tooth_location(location_string:str) -> int:
    # print(location_string)
    if location_string[0:2] == 'ul':
        return 1
    elif location_string[0:2] == 'ur':
        return 2
    elif location_string[0:2] == 'll':
        return 0
    elif location_string[0:2] == 'lr':
        return 3
    Logger.Error('bolt_nut.py   get_row_from_tooth_location()')
    Logger.Print('location_string', location_string)
    return -1


