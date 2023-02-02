from flask import Blueprint, render_template, request,flash, session, redirect,url_for
from database.db_api import db_User

# web_user = Blueprint('web_user', __name__, url_prefix='/user')
web_user = Blueprint('web_user', __name__,template_folder='templates')

twh_factory = {'221109':'山东雅乐福义齿公司'}

def check_login():
    if "user" not in session:
        return redirect(url_for('login'))

@web_user.route('/login')
def login():
    return render_template('web_user/login.html')

@web_user.route('/login_real', methods=['POST'])
def login_real():
    user = db_User.get_user(request.form.get('user_id'))
    password = request.form.get("password")
    if user is None:
        flash("没有该用户")
        return render_template('web_user/login.html')
    elif user["password"] != password:
        flash("密码错误")
        return render_template('web_user/login.html')
    else:
        session['user'] = request.form.get('user_id')
        return render_template('home.html')

@web_user.route('/sign_up')
def sign_up():
    factory_name = twh_factory[request.args.get('twh')]
    return render_template('web_user/sign_up.html', factory_name=factory_name)

@web_user.route('/sign_up_real', methods=['POST'])
def sign_up_real():
    user_in_db  = db_User.get_user(request.form.get('user_id'))
    if user_in_db is None:
        # insert into db_user
        new_user = {}
        new_user['user_id'] = request.form.get('user_id')
        new_user['factory_id'] = request.form.get('factory_id')
        new_user['password'] = request.form.get('password')
        new_user['position'] = request.form.get('position')
        db_User.table_user.insert(new_user)
        # return render_template('login.html')
        return render_template('web_user/sign_up_ok.html')
    else:
        # repeated username
        flash("该用户名已经被使用，请更换一个用户名",'error')
        # return render_template(url_for('sign_up'))
        return render_template('web_user/sign_up.html')

@web_user.route('/logout')
def log_out():
    del session['user']
    flash('已经成功登出')
    return render_template('login.html')

@web_user.route('/view_users')
def view_users():
    users = db_User.table_user.all()
    return render_template('view_users.html', users=users)

