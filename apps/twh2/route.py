# https://github.com/abalarin/Flask-on-Linode

from flask import Flask,  render_template
from wcs_robots.twh_wcs import Start_WCS_Process
from web_user.route import web_user
from web_stock.route import web_stock

app = Flask(__name__)
app.config['SECRET_KEY'] = '20221220'
app.secret_key = '20221221'   # for WTForm

app.register_blueprint(web_user)
app.register_blueprint(web_stock)

# app.debug=False
new_message = {}
new_message['test'] = {'id':2, 'payload': 'hello, dynamic message.'}


@app.route('/')
def index():
    store_map = []
    return render_template('index.html', store_map = store_map)

@app.route('/contact')
def contact():
    return render_template('cotact.html')        

@app.route('/product_wcs')
def product_wcs():
    return render_template('product_wcs.html')

@app.route('/product_acupuncture.html')
def product_acupuncture():
    return render_template('product_acupuncture.html')



Start_WCS_Process()

if __name__ == '__main__':
    # reloader or debug must be false.  
    # https://stackoverflow.com/questions/31264826/start-a-flask-application-in-separate-thread
    #  web.run(debug=True)

    app.run(host='0.0.0.0', debug=False)
