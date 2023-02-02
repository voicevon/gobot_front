from flask import Flask, redirect, url_for, render_template, request, session, flash
from flask_wtf import FlaskForm
from wtforms import StringField, IntegerField, BooleanField, SubmitField, FormField
from wtforms.validators import DataRequired, InputRequired

from database.db_api import db_Stock
from wcs_robots.twh_wcs import Start_WCS_Process

from web_user.route import web_user
from web_stock.route import web_stock

web = Flask(__name__)
web.config['SECRET_KEY'] = '20221220'
web.secret_key = '20221221'   # for WTForm


web.register_blueprint(web_user)
web.register_blueprint(web_stock)

# web.debug=False
twh_factory = {'221109':'山东雅乐福义齿公司'}

new_message = "系统正在空闲...<p>"

class MyForm(FlaskForm):
    brand = StringField('品牌', validators=[InputRequired('品牌不可空白')])
    # location_verital = boll

# @web.route('/withdraw_list', methods=['POST'])
# def withdraw_list():
#     user_request = request.json
#     print('withdraw_list()===========', user_request)
#     g_amq.Publish('twh', 'twh_withdraw', str(user_request))
#     return 'OK'

@web.route('/get_new_msg')
def get_new_message():
    return new_message

@web.route('/msgs')
def msgs():
    return render_template('message_monitor.html')

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

@web.route('/create_map_for_new_twh')
def create_map_for_new_twh():
    store_map = []
    return render_template('create_map_for_new_twh.html', store_map = store_map)



@web.route('/')
def home():
    return render_template('home.html', factory_name = twh_factory['221109'])

        
# Start_WCS_Process()

if __name__ == '__main__':
    # reloader or debug must be false.  
    # https://stackoverflow.com/questions/31264826/start-a-flask-application-in-separate-thread
    #  web.run(debug=True)

    web.run(host='0.0.0.0', debug=False)