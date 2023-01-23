


def get_row_from_location(location_string:str) -> int:
    print(location_string)
    if location_string == 'ul':
        return 1
    elif location_string == 'ur':
        return 2
    elif location_string == 'll':
        return 0
    elif location_string == 'lr':
        return 3