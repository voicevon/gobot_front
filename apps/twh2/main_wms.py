from flask import Flask,  render_template
from wcs_robots.twh_wcs import Start_WCS_Process
from web_user.route import web_user
from web_stock.route import web_stock

app = Flask(__name__)
app.config['SECRET_KEY'] = '20221220'
app.secret_key = '20221221'   # for WTForm

# app.register_blueprint(web_user)
# app.register_blueprint(web_stock)

# web.debug=False
new_message = {}
new_message['test'] = {'id':2, 'payload': 'hello, dynamic message.'}


@app.route('/create_map_for_new_twh')
def create_map_for_new_twh():
    store_map = []
    return render_template('create_map_for_new_twh.html', store_map = store_map)

        
Start_WCS_Process()

if __name__ == '__main__':
    # reloader or debug must be false.  
    # https://stackoverflow.com/questions/31264826/start-a-flask-application-in-separate-thread
    #  web.run(debug=True)

    app.run(host='0.0.0.0', debug=False)