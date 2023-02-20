from flask import Blueprint, render_template, request,flash, session, redirect,url_for
from web_user.db_api import db_User
from bolt_nut import  twh_factory
from logger import Logger
from bolt_nut import get_twh_factory
web_user = Blueprint('web_user', __name__,template_folder='templates')


def check_login():
    if "user" not in session:
        return redirect(url_for('login'))

@web_user.route('/login')
def login():
    # twh = get_twh_factory(request.args.get('twh_id'))
    # Logger.Print('@web_user.route(/login)', twh)
    return render_template('login.html')

@web_user.route('/login_real', methods=['POST'])
def login_real():
    user = db_User.get_user(request.form.get('user_id'))
    password = request.form.get("password")
    if user is None:
        flash("没有该用户")
        return render_template('login.html')
    if user["password"] != password:
        flash("密码错误")
        return render_template('login.html')
    if user['user_id'] == 'FengXuming':
        session['user'] = request.form.get('user_id')
        return render_template('home_admin.html')
    
    user_id = request.form.get('user_id')
    session['user'] = db_User.get_user(user_id)
    session['user']['factory_name'] = twh_factory[session['user']['twh_id']]
    # Logger.Print('@web_user.route(/login_real)', user)
    return redirect(url_for('web_user.home'))

@web_user.route('/sign_up')
def sign_up():
    twh = get_twh_factory(request.args.get('twh_id'))
    return render_template('sign_up.html', twh=twh)

@web_user.route('/sign_up_real', methods=['POST'])
def sign_up_real():
    user_in_db  = db_User.get_user(request.form.get('user_id'))
    if user_in_db is None:
        # insert into db_user
        
        # new_user = {}
        # new_user['user_id'] = request.form.get('user_id')
        # new_user['twh_id'] = request.form.get('twh_id')
        # new_user['password'] = request.form.get('password')
        # new_user['position'] = request.form.get('position')
        db_User.table_user.insert(request.form.to_dict())
        # return render_template('login.html')
        return render_template('sign_up_ok.html', twh=get_twh_factory(request.form.get('twh_id')))

    flash("该用户名已经被使用，请更换一个用户名", 'error')
    return render_template('sign_up.html', twh=get_twh_factory(request.form.get('twh_id')))

@web_user.route('/logout')
def log_out():
    if 'user' in session:
        del session['user']
        flash('已经成功登出')
        # twh = get_twh_factory('221109')
    return render_template('login.html')

@web_user.route('/view_users')
def view_users():
    users = db_User.table_user.all()
    return render_template('view_users.html', users=users, factory_name='山东雅乐福公司')

# @web_user.route('/')
# def home():
    # twh = get_twh_factory('221109')
    # return render_template('home.html')

# @web_user.route('/admin_home')
# def admin_home():
#     twh={}
#     twh["id"] = 1010
#     twh['factory_name'] = '山东卷积分公司'
#     return render_template('home_admin.html', twh=twh)


@web_user.route('/')
def home():
    if "user" not in session:
        return redirect(url_for("web_user.login"))
    if session['user']['position'] == '主管':
        return render_template('home_factory.html')
    else:
        return render_template('home_worker.html')