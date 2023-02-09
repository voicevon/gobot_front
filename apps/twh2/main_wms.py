from flask import Flask,  render_template, request
# from flask_wtf import FlaskForm
# from wtforms import StringField, IntegerField, BooleanField, SubmitField, FormField
# from wtforms.validators import DataRequired, InputRequired

from wcs_robots.twh_wcs import Start_WCS_Process
from web_user.route import web_user
from web_stock.route import web_stock
from bolt_nut import twh_factory

web = Flask(__name__)
web.config['SECRET_KEY'] = '20221220'
web.secret_key = '20221221'   # for WTForm

web.register_blueprint(web_user)
web.register_blueprint(web_stock)

# web.debug=False
new_message = {}
new_message['test'] = {'id':2, 'payload': 'hello, dynamic message.'}


# class MyForm(FlaskForm):
#     brand = StringField('品牌', validators=[InputRequired('品牌不可空白')])
    # location_verital = boll

# @web.route('/withdraw_list', methods=['POST'])
# def withdraw_list():
#     user_request = request.json
#     print('withdraw_list()===========', user_request)
#     g_amq.Publish('twh', 'twh_withdraw', str(user_request))
#     return 'OK'

@web.route('/create_map_for_new_twh')
def create_map_for_new_twh():
    store_map = []
    return render_template('create_map_for_new_twh.html', store_map = store_map)


@web.route('/')
def home():
    return render_template('home.html', factory_name = twh_factory['221109'])

        
Start_WCS_Process()

if __name__ == '__main__':
    # reloader or debug must be false.  
    # https://stackoverflow.com/questions/31264826/start-a-flask-application-in-separate-thread
    #  web.run(debug=True)

    web.run(host='0.0.0.0', debug=False)