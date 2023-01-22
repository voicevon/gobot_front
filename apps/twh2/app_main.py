from flask import Flask, redirect, url_for, render_template, request, session, flash
from flask_wtf import FlaskForm
from wtforms import StringField, IntegerField, BooleanField, SubmitField, FormField
from wtforms.validators import DataRequired, InputRequired
from tinydb import Query

from von.amq_agent import g_amq, g_amq_broker_config
from von.mqtt_agent import g_mqtt,g_mqtt_broker_config
from database.db_api import g_database

web = Flask(__name__)
web.config['SECRET_KEY'] = '20221220'
web.secret_key = '20221221'   # for WTForm
# web.debug=False
twh_factory = {'221109':'山东雅乐福义齿公司'}

class MyForm(FlaskForm):
    brand = StringField('品牌', validators=[InputRequired('品牌不可空白')])
    # location_verital = boll


def get_row_from_location(location_string:str) -> int:
    print(location_string)
    if location_string == 'ul':
        return 1
    elif location_string == 'ur':
        return 2
    elif location_string == 'll':
        return 0
    elif location_string == 'lr':
        return 3

@web.route('/get_stock', methods=['POST'])
def get_stock():
    # print('query_string\n\n', ss)  # https://stackoverflow.com/questions/11774265/how-do-you-access-the-query-string-in-flask-routes
    data = request.json
    index =  data.get('location_index')
    color =  data.get('color')
    print(index, color)
    q= Query()
    stocks = g_database.db_stock.search(
                                (q.brand == data.get('brand'))
                                & (q.color == data.get('color'))
                                & (q.size == data.get('size'))
                                & (q.shape == data.get('shape'))
                                & (q.location_vertical == data.get('location_vertical'))
                                & (q.location_horizontal == data.get('location_horizontal'))
                                & (q.location_index == data.get('location_index'))
                                )
    # stocks = g_database.db_stock.search((q.location_index == data.get('location_index')) & (q.color == data.get('color')))
    if len(stocks) > 0:
        # print(stocks[0])
        stocks[0]['doc_id'] = stocks[0].doc_id
        return stocks[0]
    print('get_stock()  Out of stock')
    return []

@web.route('/withdraw_list', methods=['POST'])
def withdraw_list():
    user_request = request.json
    print('withdraw_list()===========', user_request)
    g_amq.Publish('twh', 'twh_withdraw', str(user_request))
    return 'OK'

@web.route('/decrease_stock')
def decrease_stock():
    row_id = int(request.args.get('doc_id'))
    print("rrrrrrrrrrrrrrrrrrrrrrrrrrr  row_id=  ", row_id)
    q = Query()
    rows = g_database.db_stock.search(q.doc_id==row_id)
    for row in rows:
        print(row['stock_quantity'])
        row['stock_quantity'] = row['stock_quantity'] -1
        print(row['stock_quantity'])
    g_database.db_stock.write_back(rows)
    return 'OK'

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
    user = g_database.get_user(request.form.get('user_id'))
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
    user_in_db  = g_database.get_user(request.form.get('user_id'))
    if user_in_db is None:
        # insert into db_user
        new_user = {}
        new_user['user_id'] = request.form.get('user_id')
        new_user['factory_id'] = request.form.get('factory_id')
        new_user['password'] = request.form.get('password')
        new_user['position'] = request.form.get('position')
        g_database.db_user.insert(new_user)
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
    users = g_database.get_user_all()
    return render_template('view_users.html', users=users)

@web.route('/view_stocks')
def view_stocks():
    stocks = g_database.get_stock_all()
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
        print(key, user_request[key])
    request_in_stock = g_database.get_stock(user_request)
    if request_in_stock is None:
        # Can not find in stock , Try to find a empty box
        user_request['col'] = g_database.get_pure_empty_col(user_request)
        user_request['origin_quantity'] = 0
        user_request['doc_id'] = -1
    else:
        #copy request_in_stock location to user_request 
        user_request['doc_id'] = request_in_stock.doc_id
        user_request['origin_quantity'] = request_in_stock['stock_quantity']
        user_request['col'] = int(request_in_stock['col'])

    user_request['row'] = get_row_from_location(user_request['location'][0:2])
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
        g_amq.Publish('twh', 'twh_deposit', str(user_request))
        print("robot will move box to somewhere for operator........ ")
        return render_template("deposit_move.html",user_request = user_request)

@web.route('/deposit_end', methods = ['POST'])
def deposit_end():
        user_request = {}
        for key in request.form.to_dict():
            user_request[key] = request.form.get(key)
        g_database.update_stock(user_request)
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
        print(key, user_request[key])
    all_in_stock = g_database.check_stock_for_all_locations(request=user_request)
    
    if not all_in_stock:
        # Can not find in stock 
        flash("库存不足，无法开始出库，请重新下订单。")
        return  redirect(url_for("withdraw"))
    user_request['user_id'] = session['user']
    user_request['connected_box_id'] = -1
    user_request['state'] = 'idle'
    g_database.table_withdraw_queue.insert(user_request)
    return render_template('/withdraw_end.html')

@web.route('/withdraw_takeout')
def withdraw_takeout():
    if 'user' in session:
        twh = request.args.get('twh')
        box_id = g_database.get_shipout_box_id(session['user'])
        if box_id == 0:
            # not found fullfilled box
            flash("您的订单尚未备货完毕，请稍后再尝试")
            return redirect(url_for("home"))
            
        g_mqtt.publish(topic='twh/221109/shipout_box/' , payload= '{"box_id:"' + str(box_id) +',"color":"blue"}')
        return render_template('withdraw_takeout.html', twh=twh)
        
def start():
    g_mqtt_broker_config.client_id = '20221222'
    g_mqtt.connect_to_broker(g_mqtt_broker_config)                # DebugMode, must be turn off.  
    g_amq.connect_to_broker(g_amq_broker_config)
    # new_thread = Thread(target=web.run, kwargs={'debug':False, 'host':'0.0.0.0'})   # DebugMode, must be turn off.  
    # new_thread = Thread(target=web.run, kwargs={'debug':False})   # DebugMode, must be turn off.  
    # new_thread.start()
    
start()

if __name__ == '__main__':
    start()
    # twh_robot = TwhRobot_Layer("221109")

    # while True:
    #     twh_robot.spin_once()

    # reloader or debug must be false.  
    # https://stackoverflow.com/questions/31264826/start-a-flask-application-in-separate-thread
    #  web.run(debug=True)
    # while True:
    #     pass
    web.run(host='0.0.0.0', debug=False)