from tinydb import TinyDB, Query

# tutorial
# https://www.youtube.com/watch?v=aP2KJoTITO0


db = TinyDB('my_tinydb.json')
db.insert({'type': 'OSFY', 'count': 700})
db.insert({'type': 'EFY', 'count': 800})
db.all()