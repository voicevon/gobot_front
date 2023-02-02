from flask import Blueprint

# web_user = Blueprint('web_user', __name__, url_prefix='/user')
web_user = Blueprint('web_user', __name__)

@web_user.route("/uu")
def uuu():
    return 'aaa'
