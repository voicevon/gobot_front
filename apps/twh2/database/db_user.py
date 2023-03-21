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

    @classmethod
    def get_users_of_twh(cls, twh_id:str): 
        q = Query()
        users = cls.table_user.search(q.twh_id==twh_id)
        return users

    @classmethod
    def update_position(cls, user):
        doc_ids = []
        doc_ids.append(user['doc_id'])
        cls.table_user.update(user, doc_ids=doc_ids)
