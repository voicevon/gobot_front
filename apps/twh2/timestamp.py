import calendar
import time


def get_timestamp() ->int:
    current_GMT = time.gmtime()
    timestamp = calendar.timegm(current_GMT)
    return timestamp