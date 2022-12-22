import sqlite3

conn = sqlite3.connect('database/twh2.db')

c = conn.cursor()

# c.execute("""CREATE TABLE user(
#             username text,
#             password text
#             ) """)

c.execute("INSERT INTO user VALUES('feng', 'feng')")

conn.commit()

conn.close()