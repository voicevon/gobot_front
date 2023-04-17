from flask import Blueprint, render_template, request,flash, session, redirect,url_for
from database.db_user import db_User
from database.bolt_nut import  twh_factory, get_twh_factory
from logger import Logger
from operator import itemgetter
from von.mqtt_agent import g_mqtt

web_user = Blueprint('web_user', __name__,template_folder='templates')



@web_user.route('/twh/login')
def login():
    return render_template('login.html')

@web_user.route('/twh/login_real', methods=['POST'])
def login_real():
    #TODO: login record and history
    Logger.Print('web_user.sign_up_real()', request.form.to_dict())
    user_id = request.form.get('user_id')
    password = request.form.get("password")
    user = db_User.get_user(user_id)
    if user is None:
        if request.form.get('user_id') == 'FengXuming':
            if password == 'f':
                session['user']={}
                session['user']['user_id'] = user_id
                session['user']['twh_id'] = '221109'
                session['user']['position'] = 'None'
                session['user']['factory_name'] = twh_factory['221109']['name']
                return render_template('home_admin.html', twh_factory = twh_factory)
        flash("没有该用户")
        return render_template('login.html')
    if user["password"] != password:
        flash("密码错误")
        return render_template('login.html')

    
    user_id = request.form.get('user_id')
    session['user'] = db_User.get_user(user_id) # type: ignore
    session['user']['factory_name'] = twh_factory[session['user']['twh_id']]['name']
    # Logger.Print('@web_user.route(/login_real)', session)
    # "user": {
    #     "twh_id": "221109",
    #     "user_id": "jigong",
    #     "user_name": "(模拟)技工",
    #     "password": "jigong",
    #     "position": "技工",
    #     "factory_name": "卷积分义齿加工厂"
    # }
    return redirect(url_for('web_user.twh_home'))

@web_user.route('/twh/sign_up')
def sign_up():
    twh = get_twh_factory(request.args.get('twh_id'))
    return render_template('sign_up.html', twh=twh)

@web_user.route('/twh/sign_up_real', methods=['POST'])
def sign_up_real():
    user_in_db  = db_User.get_user(request.form.get('user_id'))
    if user_in_db is None:
        db_User.table_user.insert(request.form.to_dict())

        return render_template('sign_up_ok.html', twh=get_twh_factory(request.form.get('twh_id')))

    flash("该用户名已经被使用，请更换一个用户名", 'error')
    return render_template('sign_up.html', twh=get_twh_factory(request.form.get('twh_id')))

@web_user.route('/twh/logout')
def log_out():
    if 'user' in session:
        del session['user']
        flash('已经成功登出')
        # twh = get_twh_factory('221109')
    return render_template('login.html')

@web_user.route('/twh/update_position', methods=['POST'])
def update_position():
    user = request.form.to_dict()
    user['doc_id'] = int(request.form.get('doc_id'))
    db_User.update_position(user)
    flash('员工' + user['user_name'] +  ' 的职位已经更新')
    return redirect(url_for('web_user.view_users'))

@web_user.route('/twh/view_users')
def view_users():
    users = db_User.get_users_of_twh(session['user']['twh_id'])
    sorted_users = sorted(users, key=itemgetter('position')) 

    # Logger.Print('@web_user.route(/view_users)', users)
    return render_template('view_users.html', users=sorted_users)

@web_user.route('/twh')
def twh_home():
    if "user" not in session:
        return redirect(url_for("web_user.login"))
    # Logger.Print('twh_home()', session['user'])
    if session['user']['position'] == '主管':
        return render_template('home_factory.html')
    if session['user']['position'] == '技工':
        return render_template('home_worker.html')
    if session['user']['user_id'] == 'FengXuming':
        return render_template('home_admin.html',twh_factory=twh_factory)
    
    flash('您的账户已经被主管临时禁用')
    return redirect(url_for('web_user.login'))


@web_user.route('/twh/loop_porter_set_home_position')
def twh_loop_porter_set_home_position():
    # backup to database.

    # sendout to mqtt
    mqtt_topic = 'twh/221109/r0/home_position'
    mqtt_payload = request.form.get('r0')
    g_mqtt.publish(mqtt_topic, mqtt_payload)