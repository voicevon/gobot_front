


def get_row_from_tooth_location(location_string:str) -> int:
    print(location_string)
    if location_string[0:2] == 'ul':
        return 1
    elif location_string[0:2] == 'ur':
        return 2
    elif location_string[0:2] == 'll':
        return 0
    elif location_string[0:2] == 'lr':
        return 3