from flask import Flask, redirect, url_for, render_template, request, session, flash
from flask_wtf import FlaskForm
from wtforms import StringField, IntegerField, BooleanField, SubmitField, FormField
from wtforms.validators import DataRequired, InputRequired

from database.db_api import db_User,db_Stock,db_Withdraw,db_Shipout
from wcs_robots.twh_wcs import Start_WCS_Process, wcs_queue_deposit, wcs_queue_takeout
from bolt_nut import get_row_from_tooth_location
from logger import Logger

web = Flask(__name__)
web.config['SECRET_KEY'] = '20221220'
web.secret_key = '20221221'   # for WTForm
# web.debug=False
twh_factory = {'221109':'山东雅乐福义齿公司'}

class MyForm(FlaskForm):
    brand = StringField('品牌', validators=[InputRequired('品牌不可空白')])
    # location_verital = boll

# @web.route('/withdraw_list', methods=['POST'])
# def withdraw_list():
#     user_request = request.json
#     print('withdraw_list()===========', user_request)
#     g_amq.Publish('twh', 'twh_withdraw', str(user_request))
#     return 'OK'

@web.route('/decrease_stock')
def decrease_stock():
    row_id = int(request.args.get('doc_id'))
    print("rrrrrrrrrrrrrrrrrrrrrrrrrrr  row_id=  ", row_id)
    # q = Query()
    # rows = g_database.db_stock.search(q.doc_id==row_id)
    # for row in rows:
    #     print(row['stock_quantity'])
    #     row['stock_quantity'] = row['stock_quantity'] -1
    #     print(row['stock_quantity'])
    # g_database.db_stock.write_back(rows)
    return 'OK'

@web.route('/view_stock_map')
def view_stock_map():
    stock_map = db_Stock.table_stock.all()
    return render_template('view_stock_map.html', stock_map = stock_map)

@web.route('/create_map_for_new_twh')
def create_map_for_new_twh():
    store_map = []
    return render_template('create_map_for_new_twh.html', store_map = store_map)

def check_login():
    if "user" not in session:
        return redirect(url_for('login'))

@web.route('/')
def home():
    return render_template('home.html', factory_name = twh_factory['221109'])

@web.route('/login')
def login():
    return render_template('login.html')

@web.route('/login_real', methods=['POST'])
def login_real():
    user = db_User.get_user(request.form.get('user_id'))
    password = request.form.get("password")
    if user is None:
        flash("没有该用户")
        return render_template('login.html')
    elif user["password"] != password:
        flash("密码错误")
        return render_template('login.html')
    else:
        session['user'] = request.form.get('user_id')
        return render_template('home.html')

@web.route('/sign_up')
def sign_up():
    factory_name = twh_factory[request.args.get('twh')]
    return render_template('sign_up.html', factory_name=factory_name)

@web.route('/sign_up_real', methods=['POST'])
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
        return render_template('sign_up_ok.html')
    else:
        # repeated username
        flash("该用户名已经被使用，请更换一个用户名",'error')
        # return render_template(url_for('sign_up'))
        return render_template('sign_up.html')

@web.route('/logout')
def log_out():
    del session['user']
    flash('已经成功登出')
    return render_template('login.html')

@web.route('/view_users')
def view_users():
    users = db_User.table_user.all()
    return render_template('view_users.html', users=users)

@web.route('/view_stocks')
def view_stocks():
    stocks = db_Stock.table_stock.all()
    return render_template('view_stocks.html', stocks=stocks)

@web.route('/deposit')
def deposit():
    if 'user' in session:
        twh = request.args.get('twh')
        return render_template('deposit.html', twh = twh)
    else:
        return redirect(url_for('login'))

@web.route('/deposit_request', methods = ['POST'])
def deposit_request():
    user_request = {}
    # https://stackoverflow.com/questions/23205577/python-flask-immutablemultidict
    for key in request.form.to_dict():
        user_request[key] = request.form.get(key)
        # print(key, user_request[key])
    request_in_stock = db_Stock.get_stock(user_request)
    if request_in_stock is None:
        # Can not find in stock , Try to find a empty box
        # solution A:  Find empty box
        # user_request['col'] = db_Stock.get_pure_empty_col(user_request)
        user_request['origin_quantity'] = 0
        user_request['doc_id'] = -1
    else:
        #copy request_in_stock location to user_request 
        user_request['doc_id'] = request_in_stock.doc_id
        user_request['origin_quantity'] = request_in_stock['stock_quantity']
        user_request['col'] = int(request_in_stock['col'])

    user_request['row'] = get_row_from_tooth_location(user_request['location'])
    user_request['layer'] = int(user_request['location'][3:4])

    print(user_request)

    return render_template("deposit_request.html",user_request = user_request)

@web.route('/deposit_move', methods = ['POST'])
def deposit_move():
    if request.method == 'POST':
        request_form = request.form
        user_request ={}
        for key in request_form.to_dict():
            user_request[key] = request_form.get(key)
        # g_database.append_deposit(user_request)
        # g_amq.Publish('twh', 'twh_deposit', str(user_request))
        wcs_queue_deposit.put(user_request)
        print("robot will move box to somewhere for operator........ ")
        return render_template("deposit_move.html",user_request = user_request)

@web.route('/deposit_end', methods = ['POST'])
def deposit_end():
        user_request = {}
        for key in request.form.to_dict():
            user_request[key] = request.form.get(key)
        db_Stock.update_stock(user_request)
        return render_template("deposit_end.html")

@web.route('/withdraw')
def withdraw():
    if 'user' in session:
        twh = request.args.get('twh')
        return render_template('withdraw.html', twh=twh)
    else:
        return redirect(url_for('login'))

@web.route('/withdraw_end', methods = ['POST'])
def withdraw_end():
    user_request = {}
    for key in request.form.to_dict():
        user_request[key] = request.form.get(key)
        # print(key, user_request[key])
    all_in_stock = db_Stock.check_stock_for_all_locations(request=user_request)
    
    if not all_in_stock:
        # Can not find in stock 
        flash("库存不足，无法开始出库，请重新下订单。")
        return  redirect(url_for("withdraw"))
    user_request['user_id'] = session['user']
    db_Withdraw.table_withdraw_history.insert(user_request)

    user_request['connected_box_id'] = -1
    db_Withdraw.insert_withdraw_queue_multi_rows(user_request)
    Logger.Debug('aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa')
    return render_template('/withdraw_end.html')

@web.route('/withdraw_takeout')
def withdraw_takeout():
    if 'user' in session:
        twh = request.args.get('twh')
        box_id = db_Shipout.get_shipout_box_id(session['user'])
        if box_id == -1:
            # not found fullfilled box
            flash("您的订单尚未备货完毕，请稍后再尝试")
            return redirect(url_for("home"))

        db_Shipout.Update_shipout_request(session['user'])
        # takout_message = {}
        # takout_message['box_id'] = box_id
        # takout_message['user_id'] = session['user']
        wcs_queue_takeout.put(box_id)
        # The following process:
        # 1. WCS get box_id from  database.
        # 2. WCS sned box_id to shipout_box 
        # 3. The blue light will turn on. 
        # 4. User press blue button, a button_pressed message send to WCS.
        # 5. WCS free the box.
        # g_mqtt.publish(topic='twh/221109/shipout_box/command' , payload= '{"box_id:"' + str(box_id) +',"color":"blue"}')
        return render_template('withdraw_takeout.html', twh=twh)
        
Start_WCS_Process()

if __name__ == '__main__':
    # reloader or debug must be false.  
    # https://stackoverflow.com/questions/31264826/start-a-flask-application-in-separate-thread
    #  web.run(debug=True)

    web.run(host='0.0.0.0', debug=False)