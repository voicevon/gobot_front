from tinydb import TinyDB,Query

class db_User():
    table_user = TinyDB('database/twh_user.json')

    @classmethod
    def get_user(cls, user_id:str):
        q = Query()
        users = cls.table_user.search(q.user_id==user_id)
        if len(users) > 0:
            return users[0]
        return None


    # def get_user_all(self):
    #     return self.table_user.all()
