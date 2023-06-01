# https://github.com/abalarin/Flask-on-Linode

from flask import Flask,  render_template
from twh_wcs.wcs_main import Start_TwhWcs_Process
from twh_user.route import web_user
from twh_stock.route import web_stock
# from von.ocr.ocr_factory import OcrFactory
# from kvm_ocr_cloud.ocr_node_factory import OcrNodeFactory

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
    return render_template('index.html')

@app.route('/hire')
def hire():
    return render_template('hire.html')

@app.route('/hire/cpp')
def hire():
    return render_template('hire/cpp.html')

@app.route('/contact')
def contact():
    return render_template('contact.html')        

@app.route('/product_wcs')
def product_wcs():
    return render_template('wcs/product_wcs.html')

@app.route('/product_twh_m1')
def product_twh_m1():
    return render_template('wcs/twh/product_twh_m1.html')

@app.route('/product_twh_m2')
def product_twh_m2():
    return render_template('wcs/twh/product_twh_m2.html')

@app.route('/product_twh_m3')
def product_twh_m3():
    return render_template('wcs/twh/product_twh_m3.html')

@app.route('/product_twh_m4')
def product_twh_m4():
    return render_template('wcs/twh/product_twh_m4.html')

@app.route('/product_twh_rental_price')
def product_twh_rental_price():
    return render_template('wcs/twh/rental_price.html')

@app.route('/product_acupuncture')
def product_acupuncture():
    return render_template('/acupuncture/product_acupuncture.html')

@app.route('/product_iot')
def product_iot():
    return render_template('product_iot.html')

@app.route('/ocr')
def ocr():
    kvm_nodes = OcrNodeFactory.GetKnown_KvmNodeList()
    app_windows = OcrNodeFactory.GetKnown_AppWindowList()
    return render_template('ocr/index.html', kvm_nodes=kvm_nodes,app_windows=app_windows)


Start_TwhWcs_Process()

if __name__ == '__main__':
    # reloader or debug must be false.  
    # https://stackoverflow.com/questions/31264826/start-a-flask-application-in-separate-thread
    #  web.run(debug=True)
    app.run(host='0.0.0.0', debug=False)
