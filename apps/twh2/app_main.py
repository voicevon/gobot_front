from flask import Flask, redirect, url_for, render_template, request, session
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


class MyForm(FlaskForm):
    brand = StringField('品牌', validators=[InputRequired('品牌不可空白')])
    # location_verital = boll

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
    return render_template('home.html')

@web.route('/login')
def login():
    return render_template('login.html')

@web.route('/login_real', methods=['GET', 'POST'])
def login_real():
    if request.method == 'POST':
        session['user'] = request.form.get('user_id')
        return render_template('home.html')
    else:
        # wrong password
        print("login_real    xxxxxxxxxxxxxxxxxxxxxxxxxxx")
        return render_template('login.html')

@web.route('/sign_up')
def sign_up():
    return render_template('sign_up.html')


@web.route('/sign_up_real', methods=['GET', 'POST'])
def sign_up_real():
    if request.method == 'POST':
        user_in_db  = db.get_user(request.form.get('user_id'))
        if user_in_db is None:
            # insert into db_user
            new_user = {}
            new_user['user_id'] = request.form.get('user_id')
            new_user['password'] = request.form.get('password')
            g_db_api.db_user.insert(new_user)
            return render_template('login.html')
        else:
            return render_template(url_for('login_real'))

@web.route('/deposit')
def deposit():
    if 'user' in session:
        twh = request.args.get('twh')
        return render_template('deposit.html', twh = twh)
    else:
        return redirect(url_for('login'))

@web.route('/deposit_request', methods = ['POST', 'GET'])
def deposit_request():
    if request.method == 'POST':
        result = request.form
        user_request = {}
        # https://stackoverflow.com/questions/23205577/python-flask-immutablemultidict
        for key in request.form.to_dict():
            user_request[key] = request.form.get(key)
        user_request = g_database.get_stock(user_request)
        
        return render_template("deposit_request.html",user_request = user_request)

@web.route('/deposit_move', methods = ['POST', 'GET'])
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

@web.route('/deposit_end', methods = ['POST', 'GET'])
def deposit_end():
    if request.method == 'POST':
        request_form = request.form
        g_database.update_stock(request_form)
        return render_template("deposit_end.html")

@web.route('/withdraw')
def withdraw():
    if 'user' in session:
        twh = request.args.get('twh')
        return render_template('withdraw.html', twh=twh)
    else:
        return redirect(url_for('login'))

@web.route('/withdraw_end', methods = ['POST', 'GET'])
def withdraw_end():
    return render_template('withdraw_end.html')

@web.route('/withdraw_takeout')
def withdraw_takeout():
    if 'user' in session:
        twh = request.args.get('twh')
        is_in_untaken = False   # How to know?  db.table.untaken
        if is_in_untaken:
            pipe_id = 12
            g_mqtt.publish(topic="twh/221109/pipe_flash/" , payload= pipe_id)
            return render_template('withdraw_takeout.html', twh=twh)
        else:
            return render_template('withdraw_in_queue.html')
    else:
        return redirect(url_for('login'))

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